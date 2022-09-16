#pragma once

#include "Scene.hpp"

#include "Object.cpp"
#include "Pyramid.cpp"
#include "Light.cpp"
#include "Checkerboard.cpp"
#include "Plane.cpp"
#include "Material.cpp"
#include "RayHelper.cpp"
#include "LightSphere.cpp"

#define EPS 1e-4

vec3 Scene::backColor = vec3(0.7, 0.7, .8);

vector<Object*> Scene::objects = {};
vector<Light*> Scene::lights = {};

vec3 Scene::castRay(vec3 origin, vec3 dir, bool* hitGlass, int depth = 0) {
    float fogIntensity = 0.f;
    vec3 fogColor = vec3(0.6, 0, 0);
    vec3 color, normal, hit;
    Material* material;
    Object* object;

    bool isIntersect = Scene::intersect(&origin, &dir, &hit, &normal, &color, &material, &object, depth);

    if (!isIntersect || depth > 4) {
        return Scene::backColor;
    }

    if (depth == 0) {
        if (material->getName() == "glass") {
            *hitGlass = true;
            fogIntensity = dynamic_cast<Pyramid*>(object)->getFogIntensity(hit);
        } else {
            *hitGlass = false;
        }
    }

    if (!material->isReflective()) {
        return color;
    }

    vec4 albedo = material->getAlbedo();

    vec3 reflectColor, refractColor;
    float reflectWeight = 0.f;

    if (albedo[2] < EPS) {
        reflectColor = vec3(0, 0, 0);
    } else {
        vec3 reflectDir = normalize(RayHelper::reflect(dir, normal));
        vec3 reflectOrig = dot(reflectDir, normal) < 0 ? hit - normal * EPS : hit + normal * EPS;
        reflectColor = Scene::castRay(reflectOrig, reflectDir, nullptr, depth + 1);

        reflectWeight = RayHelper::SchlickApproximation(dir, normal, material->getRefractIdx());
    }

    if (albedo[3] < EPS) {
        refractColor = vec3(0, 0, 0);
    } else {
        vec3 refractDir = normalize(RayHelper::refract(dir, normal, material->getRefractIdx()));
        vec3 refractOrig = dot(refractDir, normal) < 0 ? hit - normal * EPS : hit + normal * EPS;
        refractColor = Scene::castRay(refractOrig, refractDir, nullptr, depth + 1);
        refractColor += fogColor * fogIntensity; // fog inside the crystal
    }

    float diffuseLightIntensity = 0, specularLightIntensity = 0;

    for (Light* light: Scene::lights) {
        vec3 lightDir = normalize(light->getOrigin() - hit);
        float lightDistance = (light->getOrigin() - hit).norm();

        vec3 shadowOrig = dot(lightDir, normal) < 0 ? hit - normal * EPS : hit + normal * EPS;
        vec3 shadowHit, shadowNorm, tmpColor;
        Material* tmpMaterial;
        Object* tmpObj;
        
        if (
            Scene::intersect(&shadowOrig, &lightDir, &shadowHit, &shadowNorm, &tmpColor, &tmpMaterial, &tmpObj, depth, light->getIdx()) && 
            (shadowHit - shadowOrig).norm() < lightDistance &&
            // Enable a light shine through its own sphere
            !(tmpMaterial->isLight() && dynamic_cast<LightSphere*>(tmpObj)->getIdx() == light->getIdx())
        ) {
            continue;
        }

        diffuseLightIntensity += light->getIntensity() * std::max(0.f, dot(lightDir, normal));
        specularLightIntensity += light->getIntensity() * powf(std::max(0.f, dot(-RayHelper::reflect(-lightDir, normal), dir)), material->getSpecularExp());
    }

    return
        color * diffuseLightIntensity * albedo[0] + 
        vec3(1, 1, 1) * specularLightIntensity * albedo[1] +
        reflectColor * albedo[2] * reflectWeight +
        refractColor * albedo[3] * (1 - reflectWeight);
}

bool Scene::intersect(vec3* origin, vec3* dir, vec3* hit, vec3* normal, vec3* color, Material** material, Object** object, int depth, int lightIdx) {
    float hitDist = numeric_limits<float>::max();
    bool hitScene = false;
    vec3 hitColor, hitNormal;

    Object* hitObj;

    for (Object* obj: Scene::objects) {
        // Draw a light only at straight view (depth == 0) and not as a shadow origin
        if ( 
            obj->getMaterial()->isLight() && (
                depth > 0 || 
                lightIdx != -1 && dynamic_cast<LightSphere*>(obj)->getIdx() != lightIdx
            )
        ) {
            continue;
        }
        vec3 iColor, iNormal;
        float iDist;
        if (obj->intersection(*origin, *dir, &iDist, &iColor, &iNormal) && iDist < hitDist) {
            hitScene = true;
            hitDist = iDist;
            hitNormal = iNormal;
            hitColor = iColor;
            hitObj = obj;
        }
    }

    if (!hitScene) {
        return false;
    }

    *hit = (*origin) + (*dir) * hitDist;
    *normal = hitNormal;
    *color = hitColor;
    *material = hitObj->getMaterial();
    *object = hitObj;

    return true;
}

void Scene::addObject(Object* obj) {
    Scene::objects.push_back(obj);
}

void Scene::addLight(Light* light) {
    Scene::lights.push_back(light);
}
#pragma once

#include "vec/vec3.cpp"
#include "vec/vec4.cpp"

namespace RayHelper {
    vec3 reflect(vec3 ray, vec3 normal) {
        return ray -  2.f * dot(ray, normal) * normal;
    }

    vec3 refract(vec3 ray, vec3 normal, float etaTarget, float etaIn = 1.f) { // Snell's law
        float cos1 = -dot(ray, normal);
        if (cos1 < 0.f) { // ray goes from inside the pyramid
            return refract(ray, -normal, etaIn, etaTarget);
        }

        float eta = etaIn / etaTarget;
        float sin2 = eta * sqrtf(1 - cos1 * cos1);

        if (sin2 > 1.f) { // total internal reflection
            return reflect(ray, normal);
        }

        return eta * ray + normal * (eta * cos1 - sqrtf(1 - sin2 * sin2));
    }

    float SchlickApproximation(vec3 ray, vec3 normal, float etaTarget, float etaIn = 1.f) { // Schlick
        float cos = -dot(ray, normal);
        if (cos < 0.f) { // ray goes from inside the pyramid
            return SchlickApproximation(ray, -normal, etaIn, etaTarget);
        }

        float eta = etaIn / etaTarget;

        float R0 = std::pow(eta - 1, 2) / std::pow(eta + 1, 2);

        return R0 + (1 - R0) * std::pow(1 - cos, 5);
    }

    bool triangleIntersection(vec3 orig, vec3 dir, vec3 vert1, vec3 vert2, vec3 vert3, float* distance) {
        vec3 e1 = vert2 - vert1;
        vec3 e2 = vert3 - vert1;

        vec3 pvec = cross(dir, e2);
        float det = dot(e1, pvec);

        if (det < 1e-8 && det > -1e-8) {
            return false;
        }

        float inv_det = 1 / det;
        vec3 tvec = orig - vert1;
        float u = dot(tvec, pvec) * inv_det;
        if (u < 0 || u > 1) {
            return false;
        }

        vec3 qvec = cross(tvec, e1);
        float v = dot(dir, qvec) * inv_det;
        if (v < 0 || u + v > 1) {
            return false;
        }

        float dist = dot(e2, qvec) * inv_det;

        if (dist < 0) {
            return false;
        }

        *distance = dist;
        
        return true;
    }
}
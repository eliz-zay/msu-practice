#pragma once

#include "Material.hpp"

Material::Material(string name, vec4 albedo, float specularExponent, float refractiveIdx, bool reflective = true) {
    this->name = name;
    this->reflective = reflective;
    this->light = false;
    this->albedo = albedo;
    this->specularExp = specularExponent;
    this->refractiveIdx = refractiveIdx;
}

Material::Material(string name, bool reflective, bool light) {
    this->name = name;
    this->reflective = reflective;
    this->light = light;
}

string Material::getName() {
    return this->name;
}

bool Material::isReflective() {
    return this->reflective;
}

bool Material::isLight() {
    return this->light;
}

vec4 Material::getAlbedo() {
    return this->albedo;
}

float Material::getSpecularExp() {
    return this->specularExp;
}

float Material::getRefractIdx() {
    return this->refractiveIdx;
}
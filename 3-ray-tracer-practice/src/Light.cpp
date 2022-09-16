#pragma once

#include "Light.hpp"

Light::Light(vec3 origin, float intensity, int idx) {
    this->origin = origin;
    this->intensity = intensity;
    this->idx = idx;
}

vec3 Light::getOrigin() {
    return this->origin;
}

float Light::getIntensity() {
    return this->intensity;
}

int Light::getIdx() {
    return this->idx;
}
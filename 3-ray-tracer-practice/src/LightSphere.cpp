#pragma once

#include "LightSphere.hpp"

LightSphere::LightSphere(vec3 center, float radius, int idx) {
    this->radius = radius;
    this->center = center;
    this->idx = idx;
}

bool LightSphere::intersection(vec3 origin, vec3 dir, float* distance, vec3* color, vec3* normal) {
    vec3 len = this->center - origin;
    float tca = dot(len, dir);
    float diff = dot(len, len) - pow(tca, 2);
    if (diff > pow(this->radius, 2)) {
        return false;
    }

    float thc = sqrtf(pow(this->radius, 2) - diff);

    float dist = tca - thc;
    float t1 = tca + thc;
    if (dist < 0) {
        dist = t1;
    }
    if (dist < 0) {
        return false;
    }

    *distance = dist;
    *color = vec3(1, 1, 1);
    *normal = normalize((origin + dir * dist) - this->center);

    return true;
}

int LightSphere::getIdx() {
    return this->idx;
}
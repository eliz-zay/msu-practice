#pragma once

#include <iostream>

#include "Plane.hpp"

#include "RayHelper.cpp"

using namespace std;

Plane::Plane(float width, float height, vec3 point, vec3 normal, vec3 color) {
    this->width = width;
    this->height = height;
    this->point = point;
    this->normal = normal;
    this->color = color;
}

Plane::Plane(vec3 vert0, vec3 vert1, vec3 vert2, vec3 vert3, vec3 color) {
    this->vertices.push_back(vert0);
    this->vertices.push_back(vert1);
    this->vertices.push_back(vert2);
    this->vertices.push_back(vert3);

    this->color = color;

    vec3 A = this->vertices[2] - this->vertices[0];
    vec3 B = this->vertices[1] - this->vertices[0];

    this->normal = normalize(cross(A, B));
}

bool Plane::intersection(vec3 origin, vec3 dir, float* distance, vec3* color, vec3* normal) {
    float dist1, dist2, dist;

    bool hit1 = RayHelper::triangleIntersection(origin, dir, this->vertices[0], this->vertices[1], this->vertices[2], &dist1);
    bool hit2 = RayHelper::triangleIntersection(origin, dir, this->vertices[0], this->vertices[2], this->vertices[3], &dist2);

    if (!hit1 && !hit2) {
        return false;
    }

    dist = hit1 && (!hit2 || dist1 < dist2) ? dist1 : dist2;

    *distance = dist;
    *color = this->color;
    *normal = this->normal;

    return true;
}

vec3 Plane::getNormal() {
    return this->normal;
}
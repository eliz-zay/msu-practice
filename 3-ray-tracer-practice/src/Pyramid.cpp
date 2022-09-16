#pragma once

#include <limits>
#include <cmath>

#include "Pyramid.hpp"

#include "Scene.cpp"
#include "RayHelper.cpp"

Pyramid::Pyramid(vec3 vert0, vec3 vert1, vec3 vert2, vec3 vert3, vec3 vert4, vec3 color): 
vertices(vert0, vert1, vert2, vert3, vert4) {
    for (int i = 0; i < 6; i++) {
        this->edgeColor.insert({i, color});
    }

    this->edgeCoord.insert({0, {this->vertices.upper, this->vertices.base[0], this->vertices.base[1]}});
    this->edgeCoord.insert({1, {this->vertices.upper, this->vertices.base[1], this->vertices.base[2]}});
    this->edgeCoord.insert({2, {this->vertices.upper, this->vertices.base[2], this->vertices.base[3]}});
    this->edgeCoord.insert({3, {this->vertices.upper, this->vertices.base[3], this->vertices.base[0]}});
    this->edgeCoord.insert({4, {this->vertices.base[0], this->vertices.base[2], this->vertices.base[1]}});
    this->edgeCoord.insert({5, {this->vertices.base[0], this->vertices.base[3], this->vertices.base[2]}});
}

bool Pyramid::intersection(vec3 orig, vec3 dir, float* distance, vec3* color, vec3* normal) {
    const int edgeNum = 6;

    float dist = numeric_limits<float>::max();
    bool hitPyro = false;
    int hitEdgeIdx;

    for (int i = 0; i < edgeNum; i++) {
        vector<vec3> coords = this->edgeCoord[i];
        float iDist;
        if (RayHelper::triangleIntersection(orig, dir, coords[0], coords[1], coords[2], &iDist) && iDist < dist) {
            hitPyro = true;
            dist = iDist;
            hitEdgeIdx = i;
        }
    }

    if (!hitPyro) {
        return false;   
    }

    vec3 hit = orig + dir * dist;

    if (hit == vertices.upper) { // avoid undefined reflections
        return false;
    }
    for (vec3 vertice: vertices.base) {
        if (hit == vertice) {
            return false;
        }
    }

    *distance = dist;
    *color = this->edgeColor[hitEdgeIdx];
    *normal = this->getNormal(hitEdgeIdx);

    return true;
}

vec3 Pyramid::getNormal(int edgeIdx) {
    vector<vec3> vertices = this->edgeCoord[edgeIdx];

    vec3 A = vertices[2] - vertices[0];
    vec3 B = vertices[1] - vertices[0];

    return normalize(cross(A, B));
}

float Pyramid::getFogIntensity(vec3 hit) {
    float hitLevel = hit.y();
    float upperPoint = this->vertices.upper.y();
    float baseLevel = this->vertices.base[0].y(); // all base vertices have the same level (y)

    return (upperPoint - hitLevel) * 1.f / (upperPoint - baseLevel);
}
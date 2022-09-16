#pragma once

#include "Stand.hpp"

Stand::Stand(vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec3 v4, vec3 v5, vec3 v6, vec3 v7, vec3 color) {
    this->vertices.push_back(v0);
    this->vertices.push_back(v1);
    this->vertices.push_back(v2);
    this->vertices.push_back(v3);
    this->vertices.push_back(v4);
    this->vertices.push_back(v5);
    this->vertices.push_back(v6);
    this->vertices.push_back(v7);

    this->color = color;

    this->edgeCoord.insert({0, {v3, v2, v1, v0}}); // lower
    this->edgeCoord.insert({1, {v4, v5, v6, v7}}); // upper
    this->edgeCoord.insert({2, {v0, v4, v7, v3}}); // front
    this->edgeCoord.insert({3, {v2, v6, v5, v1}}); // back
    this->edgeCoord.insert({4, {v0, v1, v5, v4}}); // left
    this->edgeCoord.insert({5, {v3, v7, v6, v2}}); // right
}

bool Stand::intersection(vec3 origin, vec3 dir, float* distance, vec3* color, vec3* normal) {
    const int edgeNum = 6;

    float dist = numeric_limits<float>::max();
    bool hitStand = false;
    int hitEdgeIdx;

    for (int i = 0; i < edgeNum; i++) {
        float dist1, dist2, edgeDist;

        vector<vec3> coord = this->edgeCoord[i];

        bool hit1 = RayHelper::triangleIntersection(origin, dir, coord[0], coord[1], coord[2], &dist1);
        bool hit2 = RayHelper::triangleIntersection(origin, dir, coord[0], coord[2], coord[3], &dist2);

        if (!hit1 && !hit2) {
            continue;
        }

        edgeDist = hit1 && (!hit2 || dist1 < dist2) ? dist1 : dist2;

        if (edgeDist < dist) {
            hitStand = true;
            dist = edgeDist;
            hitEdgeIdx = i;
        }
    }

    if (!hitStand) {
        return false;   
    }

    vec3 hit = origin + dir * dist;

    for (vec3 vertice: this->vertices) { // avoid undefined reflections
        if (hit == vertice) {
            return false;
        }
    }

    *distance = dist;
    *color = this->color;
    *normal = this->getNormal(hitEdgeIdx);

    return true;
}

vec3 Stand::getNormal(int edgeIdx) {
    vector<vec3> vertices = this->edgeCoord[edgeIdx];

    vec3 A = vertices[2] - vertices[0];
    vec3 B = vertices[1] - vertices[0];

    return normalize(cross(A, B));
}
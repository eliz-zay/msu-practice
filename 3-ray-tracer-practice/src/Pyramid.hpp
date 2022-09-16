#ifndef PYRAMID
#define PYRAMID

#include <map>
#include <vector>
#include <string>

#include "vec/vec3.cpp"
#include "vec/vec4.cpp"

#include "Object.hpp"

using namespace std;

struct Vertices {
    vec3 upper;
    vector<vec3> base;

    Vertices(vec3 vert0, vec3 vert1, vec3 vert2, vec3 vert3, vec3 vert4) {
        this->upper = vert0;
        this->base = {vert1, vert2, vert3, vert4};
    }
};

class Pyramid: public Object {
    private:
        Vertices vertices;
        map<int, vec3> edgeColor;
        map<int, vector<vec3> > edgeCoord;

    public:
        Pyramid(vec3 vert0, vec3 vert1, vec3 vert2, vec3 vert3, vec3 vert4, vec3 color);

        bool intersection(vec3 origin, vec3 dir, float* distance, vec3* color, vec3* normal);

        vec3 getNormal(int edgeIdx);
        float getFogIntensity(vec3 hit);
};

#endif
#ifndef STAND
#define STAND

#include <vector>
#include <map>

#include "vec/vec3.cpp"

#include "Object.hpp"

using namespace std;

class Stand: public Object {
    private:
        vector<vec3> vertices;
        map<int, vector<vec3> > edgeCoord;

        vec3 color;

    public:
        Stand(vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec3 v4, vec3 v5, vec3 v6, vec3 v7, vec3 color);

        bool intersection(vec3 origin, vec3 dir, float* distance, vec3* color, vec3* normal);

        vec3 getNormal(int edgeIdx);

};

#endif
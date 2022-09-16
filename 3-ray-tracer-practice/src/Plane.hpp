#ifndef PLANE
#define PLANE

#include <vector>
#include <string>

#include "vec/vec3.cpp"
#include "vec/vec4.cpp"

#include "Object.hpp"
#include "Material.hpp"

using namespace std;

class Plane: public Object {
    private:
        float width, height;
        vec3 point;

        vector<vec3> vertices;

        vec3 normal;
        vec3 color;

    public:
        Plane(float width, float height, vec3 point, vec3 normal, vec3 color);
        Plane(vec3 vert0, vec3 vert1, vec3 vert2, vec3 vert3, vec3 color);

        bool intersection(vec3 origin, vec3 dir, float* distance, vec3* color, vec3* normal);

        vec3 getNormal();
};

#endif
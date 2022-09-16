#ifndef LIGHT_SPHERE
#define LIGHT_SPHERE

#include "vec/vec4.cpp"

#include "Object.hpp"

class LightSphere: public Object {
    private:
        float radius;
        vec3 center;

        int idx;

    public:
        LightSphere(vec3 center, float radius, int idx);

        bool intersection(vec3 origin, vec3 dir, float* distance, vec3* color, vec3* normal);

        int getIdx();
};

#endif
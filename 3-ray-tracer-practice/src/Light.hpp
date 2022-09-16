#ifndef LIGHT
#define LIGHT

#include <map>
#include <vector>
#include <string>

#include "vec/vec3.cpp"
#include "vec/vec4.cpp"

class Light {
    private:
        vec3 origin;
        float intensity;

        int idx;

    public:
        Light(vec3 origin, float intensity, int idx);

        vec3 getOrigin();
        float getIntensity();

        int getIdx();
};

#endif
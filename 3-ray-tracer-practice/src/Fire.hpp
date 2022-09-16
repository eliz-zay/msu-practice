#ifndef FIRE
#define FIRE

#include "vec/vec3.cpp"

using namespace std;

class Fire: public Object {
    private:
        vec3 center;
        float radius;

        template <typename T> inline T lerp(const T &v0, const T &v1, float t);
        float sinHash(const float n);
        float noise(const vec3 &v);
        vec3 paletteFire(const float d);
        float signedDistance(const vec3 &p);
        vec3 distanceFieldNormal(const vec3 &pos);
        bool sphereTrace(const vec3 &orig, const vec3 &dir, vec3 &pos);

    public:
        Fire(vec3 center, float radius);
        
        bool intersection(vec3 origin, vec3 dir, float* distance, vec3* color, vec3* normal);
};

#endif
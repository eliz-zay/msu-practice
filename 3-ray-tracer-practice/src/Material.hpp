#ifndef MATERIAL
#define MATERIAL

#include "vec/vec3.cpp"
#include "vec/vec4.cpp"

using namespace std;

class Material {
    private:
        string name;

        bool reflective;
        bool light;

        vec4 albedo;
        float specularExp, refractiveIdx;

    public:
        Material(string name, vec4 albedo, float specularExp, float refractiveIdx, bool reflective);
        Material(string name, bool reflective, bool light); // for non-reflective material

        string getName();
        bool isReflective();
        bool isLight();
        vec4 getAlbedo();
        float getSpecularExp();
        float getRefractIdx();
};

#endif
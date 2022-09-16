#ifndef OBJECT
#define OBJECT

#include "vec/vec3.cpp"
#include "vec/vec4.cpp"

#include "Material.hpp"

using namespace std;

class Object {
    protected:
        Material* material;

    public:
        virtual bool intersection(vec3 origin, vec3 dir, float* distance, vec3* color, vec3* normal) = 0;

        void setMaterial(Material* material);

        Material* getMaterial();
};

#endif
#ifndef CHECKERBOARD
#define CHECKERBOARD

#include <vector>
#include <string>

#include "vec/vec3.cpp"
#include "vec/vec4.cpp"

#include "Object.hpp"
#include "Material.hpp"

class Checkerboard: public Object {
    private:
        std::pair<vec3, vec3> color;
        float equationY, borderX, lowBorderZ, uppBorderZ;
    
    public:
        Checkerboard(
            std::pair<vec3, vec3> color, 
            float equationY,    // everything only for horizontal board (y = equationY)
            float borderX, 
            float lowBorderZ, 
            float uppBorderZ
        );

        bool intersection(vec3 orig, vec3 dir, float* distance, vec3* color, vec3* normal);

        vec3 getNormal();
};

#endif
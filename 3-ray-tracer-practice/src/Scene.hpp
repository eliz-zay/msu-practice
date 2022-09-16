#ifndef SCENE
#define SCENE

#include <vector>
#include <string>

#include "vec/vec3.cpp"
#include "vec/vec4.cpp"

#include "Object.hpp"
#include "Pyramid.hpp"
#include "Light.hpp"
#include "Checkerboard.hpp"
#include "Plane.hpp"

class Pyramid;
class Light;
class Checkerboard;
class Plane;

using namespace std;

class Scene {
    private:
        static vec3 backColor;

        static std::vector<Object*> objects;
        static std::vector<Light*> lights;

        Scene();

    public:
        static vec3 castRay(vec3 origin, vec3 dir, bool* hitGlass, int depth);
        static bool intersect(vec3* origin, vec3* dir, vec3* hit, vec3* normal, vec3* color, Material** material, Object** object, int depth, int lightIdx = -1);

        static void addObject(Object* obj);
        static void addLight(Light* light);
};

#endif
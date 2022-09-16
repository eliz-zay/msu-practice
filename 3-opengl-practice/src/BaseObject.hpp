#ifndef BASE_OBJECT
#define BASE_OBJECT

#include <glm/glm.hpp>

#include <src/BufferManager.hpp>
#include <src/Scene.hpp>

class Scene;

enum EnumUniformType {
    GLM_MAT4,
    GLM_VEC4
};

class BaseObject {
    protected:
        GLfloat* vertices;
        GLfloat* texturePosition;
        std::map<const char*, std::tuple<GLuint, EnumUniformType, std::any>> uniform;

        BufferManager bufferManager;
        Scene* scene;

    public:
        BaseObject();
        ~BaseObject();

        void addScene(Scene* scene);
        virtual void initObject();
        template <typename T> void setUniform(const char* name, T value, EnumUniformType valueType);
        void updateUniform();
        virtual void move(double deltaTime);
        virtual void draw() = 0;
};

#endif
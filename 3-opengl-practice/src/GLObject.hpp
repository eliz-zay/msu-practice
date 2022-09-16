#ifndef GL_OBJECT
#define GL_OBJECT

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <src/BaseObject.hpp>
#include <src/ImageTexture.hpp>
#include <src/Helper.hpp>

class GLObject: public BaseObject {
    protected:
        glm::mat4 transformMatrix;

        ImageTexture* texture;
        Helper::RectCoordinates currentCoord;

    public:
        GLObject(glm::vec2 initPosition, ImageTexture* texture);
        ~GLObject();

        Helper::RectCoordinates getCurrentCoord();
        virtual void initObject();
        virtual void draw();
};

#endif
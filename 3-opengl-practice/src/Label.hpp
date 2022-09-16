#ifndef LABEL
#define LABEL

#include <glm/glm.hpp>

#include <src/BaseObject.hpp>
#include <src/BufferManager.hpp>

class Label: public BaseObject {
    private:
        std::string text;
        std::string fontSource;
        int size;
        glm::vec4 color;

    public:
        Label(std::string fontSource, int size);

        void setText(glm::vec2 position, std::string text, glm::vec4 color);
        virtual void initObject();
        virtual void draw();
};

#endif
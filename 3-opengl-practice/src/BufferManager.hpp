#ifndef BUFFER_MANAGER
#define BUFFER_MANAGER

#include <map>
#include <any>

#include <GL/glew.h>

class BufferManager {
    private:
        GLuint vao, vboPosition, vboTexture;

    public:
        ~BufferManager();
        
        void initBuffers(GLfloat* vertices, GLfloat* texturePosition, int length);
        void setContext();
        void run(GLuint texture, int offset);
};

#endif
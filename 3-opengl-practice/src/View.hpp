#ifndef VIEW
#define VIEW

#include <glm/glm.hpp>

class View {
    private:
        static int width, height;
        static glm::mat4 projection;
        static glm::mat4 lookAt;
        static glm::mat4 resultMatrix;
        
        View();

    public:
        static void setResolution(int width, int height);
        static glm::mat4* getProjection();
        static glm::mat4* getResultMatrix();
        static void move(glm::vec2 position);
};

#endif
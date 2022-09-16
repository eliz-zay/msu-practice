#ifndef SCENE
#define SCENE

#include <glm/glm.hpp>

#include <src/BaseObject.hpp>
#include <src/BaseTile.hpp>
#include <src/TrapTile.hpp>
#include <src/DoorTile.hpp>

#include <src/LabelManager.hpp>

class BaseObject;

class Scene {
    private:
        GLuint programID; 
        std::string vertexShader, fragShader;
        std::string name;
        std::vector<BaseObject*> baseObjects;
        std::vector<BaseTile*> baseTiles;
        std::vector<TrapTile*> trapTiles;
        std::vector<DoorTile*> doorTiles;
        std::vector<std::vector<char> > mazeData;
    
    public:
        Scene(std::string name, std::string mazeSource, std::string vertexShader, std::string fragShader);
        Scene(std::string name, std::string fontSource, glm::vec2 initPosition, std::string labelText, int size, glm::vec4 color, std::string vertexShader, std::string fragShader);
        ~Scene();
        
        void activate();
        void draw(double deltaTime);

        GLuint getProgramID();
};

#endif
#ifndef SPRITE
#define SPRITE

#include <vector>

#include <src/GLObject.hpp>
#include <src/BaseTile.hpp>
#include <src/TrapTile.hpp>

class Sprite: public GLObject {
    private:
        std::vector<BaseTile*> baseTiles;
        
        bool isCollision(double dx, double dy);
        
    public:
        Sprite(glm::vec2 initPosition, ImageTexture* texture);

        void setTiles(std::vector<BaseTile*> baseTiles);
        virtual void move(double deltaTime);
};

#endif
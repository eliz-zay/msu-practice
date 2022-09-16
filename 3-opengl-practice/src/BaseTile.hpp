#ifndef BASE_TILE
#define BASE_TILE

#include <src/Tile.hpp>

class BaseTile: public Tile {
    private:
        bool wall;  
        
    public:
        BaseTile(glm::vec2 initPosition, ImageTexture* texture, bool wall);

        bool isWall();
};

#endif
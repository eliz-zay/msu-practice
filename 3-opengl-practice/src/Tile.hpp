#ifndef TILE
#define TILE

#include "GLObject.hpp"

class Tile: public GLObject {
    public:
        Tile(glm::vec2 initPosition, ImageTexture* texture);
};

#endif
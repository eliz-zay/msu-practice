#ifndef TRAP_TILE
#define TRAP_TILE

#include <src/Tile.hpp>
#include <src/BaseTile.hpp>
#include <src/Sprite.hpp>

class Sprite;

class TrapTile: public Tile {
    private:
        BaseTile* altTile;

    public:
        TrapTile(glm::vec2 initPosition, ImageTexture* texture, ImageTexture* altTexture);

        virtual void draw(Sprite* mainHero);
};

#endif
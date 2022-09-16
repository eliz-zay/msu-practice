#pragma once

#include <src/BaseTile.hpp>

#include <src/Tile.cpp>

BaseTile::BaseTile(glm::vec2 initPosition, ImageTexture* texture, bool wall): 
    Tile(initPosition, texture) 
{
    this->wall = wall;
}

bool BaseTile::isWall() {
    return this->wall;
}
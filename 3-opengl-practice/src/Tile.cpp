#pragma once

#include <src/Tile.hpp>

#include <src/GLObject.cpp>

Tile::Tile(glm::vec2 initPosition, ImageTexture* texture): 
    GLObject(initPosition, texture) 
{
}
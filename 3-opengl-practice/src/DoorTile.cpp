#pragma once

#include <src/DoorTile.hpp>

#include <src/Game.cpp>
#include <src/Tile.cpp>
#include <src/Sprite.cpp>

DoorTile::DoorTile(glm::vec2 initPosition, ImageTexture* texture):
    Tile(initPosition, texture)
{
}

void DoorTile::isOpened(Sprite* mainHero) {
    GLfloat x1 = (this->currentCoord.x1 + this->currentCoord.x0) / 2;
    GLfloat y1 = (this->currentCoord.y1 + this->currentCoord.y0) / 2;
    GLfloat x2 = (mainHero->getCurrentCoord().x1 + mainHero->getCurrentCoord().x0) / 2;
    GLfloat y2 = (mainHero->getCurrentCoord().y1 + mainHero->getCurrentCoord().y0) / 2;

    if (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)) <= 20) {
        Game::update(SceneChange::NEXT_LEVEL);
    }
}

void DoorTile::draw(Sprite* mainHero) {
    this->updateUniform();
    this->bufferManager.setContext();
    this->bufferManager.run(this->texture->getID(), 0);
    this->isOpened(mainHero);
}
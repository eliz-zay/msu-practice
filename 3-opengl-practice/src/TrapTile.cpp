#pragma once

#include <src/TrapTile.hpp>

#include <src/Tile.cpp>
#include <src/BaseTile.cpp>
#include <src/Sprite.cpp>
#include <src/Game.cpp>

TrapTile::TrapTile(glm::vec2 initPosition, ImageTexture* texture, ImageTexture* altTexture):
    Tile(initPosition, texture)
{
    altTile = new BaseTile(initPosition, altTexture, false);
}

void TrapTile::draw(Sprite* mainHero) {
    this->updateUniform();
    this->bufferManager.setContext();

    GLfloat x1 = (this->currentCoord.x1 + this->currentCoord.x0) / 2;
    GLfloat y1 = (this->currentCoord.y1 + this->currentCoord.y0) / 2;
    GLfloat x2 = (mainHero->getCurrentCoord().x1 + mainHero->getCurrentCoord().x0) / 2;
    GLfloat y2 = (mainHero->getCurrentCoord().y1 + mainHero->getCurrentCoord().y0) / 2;

    double distance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));

    if (distance <= 20) {
        this->bufferManager.run(this->texture->getID(), 0);
        Game::update(SceneChange::DEATH);
    } else if (distance <= 70) {
        this->bufferManager.run(this->texture->getID(), 0);
    } else {
        this->altTile->draw();
    }
}
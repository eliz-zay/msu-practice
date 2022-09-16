#pragma once

#include <src/Sprite.hpp>
#include <src/Helper.hpp>

#include <src/BaseTile.cpp>
#include <src/Window.cpp>

Sprite::Sprite(glm::vec2 initPosition, ImageTexture* texture):
    GLObject(initPosition, texture)
{
}

void Sprite::setTiles(std::vector<BaseTile*> baseTiles) {
    this->baseTiles = baseTiles;
}

void Sprite::move(double deltaTime) {
    GLFWwindow* window = Window::getWindow();

    deltaTime /= 20;
    float speed = 4.f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && this->currentCoord.y0 > 0 && !this->isCollision(0, -deltaTime * speed)) {
        this->transformMatrix = glm::translate(this->transformMatrix, glm::vec3(0.f, -deltaTime * speed, 0.f));
        this->currentCoord.y0 -= deltaTime * speed;
        this->currentCoord.y1 -= deltaTime * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !this->isCollision(0, deltaTime * speed)) {
        this->transformMatrix = glm::translate(this->transformMatrix, glm::vec3(0.f, deltaTime * speed, 0.f));
        this->currentCoord.y0 += deltaTime * speed;
        this->currentCoord.y1 += deltaTime * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && !this->isCollision(deltaTime * speed, 0)) {
        this->transformMatrix = glm::translate(this->transformMatrix, glm::vec3(deltaTime * speed, 0.f, 0.f));
        this->currentCoord.x0 += deltaTime * speed;
        this->currentCoord.x1 += deltaTime * speed;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && this->currentCoord.x0 > 0 && !this->isCollision(-deltaTime * speed, 0)) {
        this->transformMatrix = glm::translate(this->transformMatrix, glm::vec3(-deltaTime * speed, 0.f, 0.f));
        this->currentCoord.x0 -= deltaTime * speed;
        this->currentCoord.x1 -= deltaTime * speed;
    }

    GLfloat x = (this->currentCoord.x1 + this->currentCoord.x0) / 2;
    GLfloat y = (this->currentCoord.y1 + this->currentCoord.y0) / 2;

    View::move(glm::vec2(x, y));
}

bool Sprite::isCollision(double dx, double dy) {
    Helper::RectCoordinates newCoord({
        this->currentCoord.x0 + dx, this->currentCoord.y0 + dy,
        this->currentCoord.x1 + dx, this->currentCoord.y1 + dy
    });
    for (auto pTile: this->baseTiles) {
        if (pTile->isWall()) {
            Helper::RectCoordinates wallCoord = pTile->getCurrentCoord();
            if (
                newCoord.x0 < wallCoord.x1 &&
                newCoord.x1 > wallCoord.x0 &&
                newCoord.y1 > wallCoord.y0 &&
                newCoord.y0 < wallCoord.y1 
            ) {
                return true;
            }
        }
    }

    return false;
}
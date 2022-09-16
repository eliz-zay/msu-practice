#pragma once

#include <stdio.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <src/GLObject.hpp>

#include <src/BaseObject.cpp>
#include <src/ImageTexture.cpp>
#include <src/BufferManager.cpp>
#include <src/View.cpp>

GLObject::GLObject(glm::vec2 initPosition, ImageTexture* texture):
    BaseObject()
{
    this->texture = texture;

    int width = this->texture->getWidth();
    int height = this->texture->getHeight();

    this->vertices = new GLfloat [18] {
        initPosition.x, initPosition.y + height, 0.f,
        initPosition.x, initPosition.y, 0.f,
        initPosition.x + width, initPosition.y, 0.f,

        initPosition.x, initPosition.y + height, 0.f,
        initPosition.x + width, initPosition.y, 0.f,
        initPosition.x + width, initPosition.y + height, 0.f
    };

    this->transformMatrix = glm::mat4(1.f);

    this->currentCoord = Helper::RectCoordinates({
        initPosition.x, initPosition.y, 
        initPosition.x + width, initPosition.y + height
    });

    this->bufferManager.initBuffers(this->vertices, this->texturePosition, 1);
}

GLObject::~GLObject() {
    delete[] this->vertices;
}

Helper::RectCoordinates GLObject::getCurrentCoord() {
    return this->currentCoord;
}

void GLObject::initObject() {
    this->setUniform<glm::mat4*>("resultMatrix", View::getResultMatrix(), EnumUniformType::GLM_MAT4);
    this->setUniform<glm::mat4*>("transform", &(this->transformMatrix), EnumUniformType::GLM_MAT4);
}

void GLObject::draw() {
    this->updateUniform();
    this->bufferManager.setContext();
    this->bufferManager.run(this->texture->getID(), 0);
}
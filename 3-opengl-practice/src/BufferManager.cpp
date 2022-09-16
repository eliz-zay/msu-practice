#pragma once

#include <iostream>

#include "glm/ext.hpp"
#include <glm/gtx/string_cast.hpp>

#include <src/BufferManager.hpp>

#include <src/TextLib.cpp>

BufferManager::~BufferManager() {
	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, &this->vboPosition);
	glDeleteBuffers(1, &this->vboTexture);
    glDeleteVertexArrays(1, &this->vao);
}

void BufferManager::initBuffers(GLfloat* vertices, GLfloat* texturePosition, int length) {
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vboPosition);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboPosition);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 18 * length, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glGenBuffers(1, &this->vboTexture);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboTexture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 * length, NULL, GL_STATIC_DRAW);
    for (int i = 0; i < length; i++) {
        glBufferSubData(GL_ARRAY_BUFFER, i * 12 * sizeof(GLfloat), sizeof(GLfloat) * 12, texturePosition);
    }

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
}

void BufferManager::setContext() {
	glBindVertexArray(this->vao);
}

void BufferManager::run(GLuint texture, int offset) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, offset, 6);
}
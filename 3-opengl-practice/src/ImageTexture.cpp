#pragma once

#include <exception>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <src/ImageTexture.hpp>

ImageTexture::ImageTexture(std::string source) {
    this->data = stbi_load(source.c_str(), &this->width, &this->height, &this->channels, 0);

	if (!this->data) {
		throw std::invalid_argument("ImageTexture: Failed to load texture");
	}
}

ImageTexture::~ImageTexture() {
    stbi_image_free(this->data);
}

void ImageTexture::createTexture() {
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);

	GLenum format = this->channels == 3 ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, this->data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

unsigned int ImageTexture::getID() {
    return this->textureID;
}

int ImageTexture::getWidth() {
    return this->width;
}

int ImageTexture::getHeight() {
    return this->height;
}
#pragma once

#include <src/BaseObject.hpp>

BaseObject::BaseObject():
    bufferManager()
{
    this->texturePosition = new GLfloat [12] {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    };
}

BaseObject::~BaseObject() {
    delete[] this->texturePosition;
}

void BaseObject::addScene(Scene* scene) {
    this->scene = scene;
}

void BaseObject::initObject() {
	this->setUniform<glm::mat4*>("resultMatrix", View::getResultMatrix(), EnumUniformType::GLM_MAT4);
}

template <typename T>
void BaseObject::setUniform(const char* name, T value, EnumUniformType valueType) {
    GLuint programID = scene->getProgramID();
	this->uniform.emplace(
		name,
		std::make_tuple(glGetUniformLocation(programID, name), valueType, value)
	);
}

void BaseObject::updateUniform() {
	for (const auto& kv: this->uniform) {
		std::tuple<GLuint, EnumUniformType, std::any> value = kv.second;
		EnumUniformType type = std::get<1>(value);

		if (type == EnumUniformType::GLM_MAT4) {
			glm::mat4* uniformValue = std::any_cast<glm::mat4*>(std::get<2>(value));
			glUniformMatrix4fv(std::get<0>(value), 1, GL_FALSE, &((*uniformValue)[0][0]));
		} else if (type == EnumUniformType::GLM_VEC4) {
			glm::vec4* uniformValue = std::any_cast<glm::vec4*>(std::get<2>(value));
			glUniform4fv(std::get<0>(value), 1, &((*uniformValue)[0]));
		}
	}
}

void BaseObject::move(double deltaTime) {
    // Do nothing
}
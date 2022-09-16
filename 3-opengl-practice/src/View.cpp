#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "View.hpp"

int View::width = 0;
int View::height = 0;
glm::mat4 View::projection = glm::mat4(1.f);
glm::mat4 View::lookAt = glm::mat4(1.f);
glm::mat4 View::resultMatrix = glm::mat4(1.f);

void View::setResolution(int width, int height) {
    View::width = width;
    View::height = height;

    View::projection = 
        glm::rotate(glm::mat4(1.f), glm::radians(180.f), glm::vec3(1.f, 0.f, 0.f)) * 
        glm::translate(glm::mat4(), glm::vec3(-1.f, -1.f, 0.f)) *
        glm::scale(glm::vec3(2./(View::width), 2./(View::height), 1.f));

    View::resultMatrix = projection * lookAt;
}

void View::move(glm::vec2 position) {
    glm::vec2 newPos(
        -position.x + View::width / 2,
        -position.y + View::height / 2
    );
    lookAt = glm::translate(
        glm::mat4(1.f),
        glm::vec3(
            newPos.x,
            newPos.y,
            0.f
        )
    );

    View::resultMatrix = projection * lookAt;
}

glm::mat4* View::getProjection() {
    return &(View::projection);
}

glm::mat4* View::getResultMatrix() {
    return &(View::resultMatrix);
}
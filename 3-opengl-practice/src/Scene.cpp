#pragma once

#include <vector>

#include <src/shader/shader.hpp>

#include <src/Scene.hpp>

#include <src/Sprite.cpp>
#include <src/BaseTile.cpp>
#include <src/TrapTile.cpp>
#include <src/DoorTile.cpp>
#include <src/Label.cpp>
#include <src/MazeHelper.cpp>
#include <src/LabelManager.cpp>

Scene::Scene(std::string name, std::string mazeSource, std::string vertexShader, std::string fragShader) {
    this->name = name;
    this->vertexShader = vertexShader;
    this->fragShader = fragShader;

    this->programID = LoadShaders(this->vertexShader.c_str(), this->fragShader.c_str());

    std::vector<std::vector<char> > mazeData = MazeHelper::parseMazeData(mazeSource);
    MazeHelper::mazeDataToGLObjects(mazeData, &this->baseObjects, &this->baseTiles, &this->trapTiles, &this->doorTiles);

    for (auto obj: this->baseObjects) {
        obj->addScene(this);
        obj->initObject();
    }
    for (auto tile: this->baseTiles) {
        tile->addScene(this);
        tile->initObject();
    }
    for (auto tile: this->doorTiles) {
        tile->addScene(this);
        tile->initObject();
    }
    for (auto tile: this->trapTiles) {
        tile->addScene(this);
        tile->initObject();
    }
}

Scene::Scene(std::string name, std::string fontSource, glm::vec2 initPosition, std::string labelText, int size, glm::vec4 color, std::string vertexShader, std::string fragShader) {
    this->name = name;
    this->vertexShader = vertexShader;
    this->fragShader = fragShader;

    this->programID = LoadShaders(this->vertexShader.c_str(), this->fragShader.c_str());

    Label* label = LabelManager::createLabel(fontSource, color, this->name, initPosition, labelText, size);
    this->baseObjects.push_back(label);
    label->addScene(this);
    label->initObject();
}

Scene::~Scene() {
    glDeleteProgram(this->programID);
}

void Scene::activate() {
    std::tuple<glm::vec2, std::string, glm::vec4> labelText = LabelManager::getSceneText(this->name);
    if (std::get<1>(labelText) == "") {
        // This is not a Label scene
        return;
    }

    (dynamic_cast<Label*>(this->baseObjects[0])->setText(std::get<0>(labelText), std::get<1>(labelText), std::get<2>(labelText)));
}

void Scene::draw(double deltaTime) {
    glUseProgram(this->programID);
    for (auto obj: this->baseObjects) {
        obj->move(deltaTime);
    }
    for (auto tile: this->baseTiles) {
        tile->draw();
    }
    for (auto tile: this->doorTiles) {
        tile->draw(dynamic_cast<Sprite*>(this->baseObjects[0]));
    }
    for (auto tile: this->trapTiles) {
        tile->draw(dynamic_cast<Sprite*>(this->baseObjects[0]));
    }
    for (auto obj: this->baseObjects) {
        obj->draw();
    }
}

GLuint Scene::getProgramID() {
    return this->programID;
}
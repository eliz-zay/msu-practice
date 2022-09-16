#pragma once

#include <src/LabelManager.hpp>

#include <src/Label.cpp>

map<pair<string, int>, Label*> LabelManager::labelInstances = {};
map<string, tuple<glm::vec2, string, glm::vec4>> LabelManager::labelText = {};

Label* LabelManager::createLabel(string fontSource, glm::vec4 color, string sceneName, glm::vec2 position, string text, int size) {
    if (LabelManager::labelInstances.find(make_pair(fontSource, size)) == LabelManager::labelInstances.end()) {

        Label* label = new Label(fontSource, size);

        LabelManager::labelInstances.insert({make_pair(fontSource, size), label});
        LabelManager::labelText.insert({sceneName, make_tuple(position, text, color)});

        return label;

    } else {
        LabelManager::labelText.insert({sceneName, make_tuple(position, text, color)});
        return LabelManager::labelInstances[{fontSource, size}];
    }
}

tuple<glm::vec2, string, glm::vec4> LabelManager::getSceneText(string sceneName) {
    return LabelManager::labelText[sceneName];
}
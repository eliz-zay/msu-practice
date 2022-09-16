#pragma once

#include <src/Game.hpp>

#include <src/Window.cpp>
#include <src/View.cpp>
#include <src/Scene.cpp>
#include <src/ImageTexture.cpp>

std::map<std::string, std::string> Game::textureSources = {};
std::map<std::string, ImageTexture*> Game::imageTextures = {};
std::tuple<std::string, Scene*, int, int> Game::activeScene = {};
std::map<std::string, Scene*> Game::scenes = {};
std::map<int, std::string> Game::levelScenes = {};
std::map<int, std::string> Game::levelIntros = {};


void Game::update(SceneChange sceneChange) {
    GLFWwindow* window = Window::getWindow();

    std::string name = std::get<0>(Game::activeScene);
    int level = std::get<2>(Game::activeScene);
    int levelIntro = std::get<3>(Game::activeScene);

    bool enterDown = Window::isKeyDown(Key::ENTER);

    if (enterDown && name == "Intro") {
        Game::setActiveScene(Game::levelIntros[1], 0, 1);
    } else if (enterDown && levelIntro != 0) {
        Game::setActiveScene(Game::levelScenes[levelIntro], levelIntro, 0);
    }

    if (sceneChange == SceneChange::NEXT_LEVEL && (!Game::levelIntros[level + 1].empty())) {
        Game::setActiveScene(Game::levelIntros[level + 1], 0, level + 1);
    } else if (sceneChange == SceneChange::NEXT_LEVEL || sceneChange == SceneChange::WIN) {
        Game::setActiveScene("Win");
    } else if (sceneChange == SceneChange::DEATH) {
        Game::setActiveScene("Death");
    }
}

void Game::play() {
    double lastTime = 0.;
    double lastTimeFPS = 0.;
    double currentTime, deltaTime;
    int frames = 0;
    do {
        currentTime = Window::getTime();
        deltaTime = 1000 * (currentTime - lastTime);
        lastTime = currentTime;
        frames++;
        if (currentTime - lastTimeFPS >= 1.) {
            std::cout << double(frames) << " fps" << std::endl;
            frames = 0;
            lastTimeFPS += 1.;
        }

        Window::clearWindow();
        Game::update();
        std::get<1>(Game::activeScene)->draw(deltaTime);
        Window::refreshWindow();
    } while (Window::shouldBeOpened());
}

void Game::end() {
    Window::closeWindow();
}

void Game::setResolution(int width, int height) {
    View::setResolution(width, height);
    Window::initWindow(width, height);
}

void Game::setTextureSources() {
    Game::textureSources.insert({"main_hero", "resources/sprites/main_hero1.png"});
    Game::textureSources.insert({"floor", "resources/floor/plain_floor.jpg"});
	Game::textureSources.insert({"wall", "resources/walls/wall.jpg"});
    Game::textureSources.insert({"door", "resources/doors/portal.jpg"});
    Game::textureSources.insert({"trap", "resources/other/chemicals.png"});
}

void Game::loadTextures() {
    for (auto kv: Game::textureSources) {
        Game::imageTextures.insert({kv.first, new ImageTexture(kv.second)});
    }
    for (auto kv: Game::imageTextures) {
        kv.second->createTexture();
    }
}

ImageTexture* Game::getTexture(std::string name) {
    return Game::imageTextures[name];
}

void Game::addScene(std::string name, int level, int levelIntro, Scene* scene) {
    Game::scenes.insert({name, scene});
    if (level) {
        Game::levelScenes.insert({level, name});
    } else if (levelIntro) {
        Game::levelIntros.insert({levelIntro, name});
    }
}

void Game::setActiveScene(std::string name, int level, int levelIntro) {
    Game::activeScene = {name, Game::scenes[name], level, levelIntro};
    Game::scenes[name]->activate();
}
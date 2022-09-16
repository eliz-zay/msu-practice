#pragma once

#include <fstream>
#include <vector>
#include <exception>

#include <glm/glm.hpp>

#include <src/MazeHelper.hpp>

#include <src/Game.cpp>
#include <src/ImageTexture.cpp>
#include <src/BaseObject.cpp>
#include <src/Sprite.cpp>
#include <src/BaseTile.cpp>
#include <src/TrapTile.cpp>
#include <src/DoorTile.cpp>

std::vector<std::vector<char> > MazeHelper::parseMazeData(std::string mazeFile) {
    std::vector<std::vector<char> > mazeData;
    std::string line;
    std::ifstream file;

    file.open(mazeFile.c_str());
    if (!file.is_open()) {
        throw std::invalid_argument("MazeHelper: Could not open file");
    }

    while (std::getline(file, line)) {
        mazeData.push_back(std::vector<char>(line.begin(), line.end()));
    }

    file.close();
    return mazeData;
}

void MazeHelper::mazeDataToGLObjects(
    std::vector<std::vector<char> > mazeData,
    std::vector<BaseObject*>* sprites,
    std::vector<BaseTile*>* baseTiles,
    std::vector<TrapTile*>* trapTiles,
    std::vector<DoorTile*>* doorTiles
) {
    ImageTexture* floor = Game::getTexture("floor");
    ImageTexture* wall = Game::getTexture("wall");
    ImageTexture* door = Game::getTexture("door");
    ImageTexture* trap = Game::getTexture("trap");
    ImageTexture* mainHero = Game::getTexture("main_hero");
    
    // Here all tiles have the same size
    int width = floor->getWidth();
    int height = floor->getHeight();

    for (int i = 0; i < mazeData.size(); i++) {
        for (int j = 0; j < mazeData[i].size(); j++) {
            switch (mazeData[i][j]) {
                case ('@'): {
                    (*sprites).push_back(
                        new Sprite(
                            glm::vec2(j * width * 1.f, i * height * 1.f),
                            mainHero
                        )
                    );
                }
                case ('.'): {
                    (*baseTiles).push_back(
                        new BaseTile(
                            glm::vec2(j * width * 1.f, i * height * 1.f), 
                            floor, 
                            false
                        )
                    );
                    break; 
                }
                case ('#'): {
                    (*baseTiles).push_back(
                        new BaseTile(
                            glm::vec2(j * width * 1.f, i * height * 1.f), 
                            wall, 
                            true
                        )
                    ); 
                    break;
                }
                case ('T'): {
                    (*trapTiles).push_back(
                        new TrapTile(
                            glm::vec2(j * width * 1.f, i * height * 1.f),
                            trap,
                            floor
                        )
                    );
                    break;
                }
                case ('x'): {
                    (*doorTiles).push_back(
                        new DoorTile(
                            glm::vec2(j * width * 1.f, i * height * 1.f),
                            door
                        )
                    );
                    break;
                }
            }
        }
    }

    for (auto sprite: *sprites) {
        dynamic_cast<Sprite*>(sprite)->setTiles(*baseTiles);
    }
}
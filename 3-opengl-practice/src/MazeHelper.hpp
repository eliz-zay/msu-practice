#ifndef MAZE_HELPER
#define MAZE_HELPER

#include <src/BaseObject.hpp>
#include <src/BaseTile.hpp>
#include <src/TrapTile.hpp>
#include <src/DoorTile.hpp>

namespace MazeHelper {
    std::vector<std::vector<char> > parseMazeData(std::string mazeFile);
    void mazeDataToGLObjects(
        std::vector<std::vector<char> > mazeData,
        std::vector<BaseObject*>* sprites,
        std::vector<BaseTile*>* baseTiles,
        std::vector<TrapTile*>* trapTiles,
        std::vector<DoorTile*>* doorTiles
    );
    std::map<std::string, std::string> getTextureSources();
}

#endif
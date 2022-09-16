#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#include "../shared/SystemHelper.cpp"

using namespace std;

namespace Config {
    const string STORAGE_LOCATION = SystemHelper::getCurrentDirectory() + "/db/";
    const string TABLE_FILE_EXTENSION = ".table";
}

#endif
#pragma once

#include <unistd.h>
#include <limits.h>

#include <string>

using namespace std;

namespace SystemHelper {
    string getCurrentDirectory() {
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));

        return cwd;
    }
}
#ifndef LABEL_MANAGER
#define LABEL_MANAGER

#include <string>
#include <glm/glm.hpp>

#include <src/Label.hpp>

using namespace std;

class LabelManager {
    private:
        static map<pair<string, int>, Label*> labelInstances;
        static map<string, tuple<glm::vec2, string, glm::vec4>> labelText;

        LabelManager();

    public:
        static Label* createLabel(string fontSource, glm::vec4 color, string sceneName, glm::vec2 position, string text, int size);
        static tuple<glm::vec2, string, glm::vec4> getSceneText(string sceneName);
};

#endif
#ifndef TEXT_HELPER
#define TEXT_HELPER

#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    unsigned int textureId;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class TextLib {
    private:
        static std::map<char, Character*> characters;
        static std::string fontSource;
        
        static FT_Library lib;

    public:
        static void initFont(std::string pFontSource, int fontSize);
        static Character* getChar(char c);
};
#endif
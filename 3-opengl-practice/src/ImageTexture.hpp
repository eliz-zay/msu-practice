#ifndef IMAGE_TEXTURE
#define IMAGE_TEXTURE

class ImageTexture {
    private:
        unsigned int textureID;
        int width, height, channels;
        unsigned char* data;

    public:
        ImageTexture(std::string source);
        ~ImageTexture();

        void createTexture();

        unsigned int getID();
        int getWidth();
        int getHeight();
};

#endif
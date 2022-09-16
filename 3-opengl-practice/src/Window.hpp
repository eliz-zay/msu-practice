#ifndef WINDOW
#define WINDOW

#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum Key {
    ENTER
};

class Window {
    private:
        static int width, height;
        static GLFWwindow* window;

        static std::map<Key, bool> keydown;

        Window();
        
    public:
        static void initWindow(int width, int height);
        static void clearWindow();
        static bool shouldBeOpened();
        static void refreshWindow();
        static void closeWindow();

        static bool isKeyDown(Key key);

        static GLFWwindow* getWindow();
        static int getWidth();
        static int getHeight();
        static double getTime();
};

#endif
/**
 * main.cpp
 *
 * @author mdq3
 */

#include <iostream>
#include <GL/glew.h>
#include "../include/Window.hpp"

int main(int argc, char* argv[])
{
    //std::cout << glGetString(GL_VERSION) << '\n';
    Window window(1200, 1000);
    while(window.isRunning())
    {
        window.handleEvents();
        window.renderScene();
    }
    window.close();
    return 0;
}

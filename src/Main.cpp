
#include <iostream>
#include <GL/glew.h>
#include "../include/Window.hpp"
#include "../include/Main.hpp"

Main::Main(int windowWidth, int windowHeight) :
window{windowWidth, windowHeight}
{
    mainLoop();
}

Main::~Main() {}

void Main::mainLoop()
{
    while(window.isRunning())
    {
        window.handleEvents();
        window.renderScene();
    }
    window.close();
}

int main(int argc, char* argv[])
{
    Main program(1200, 1000);
    //std::cout << glGetString(GL_VERSION) << '\n';
    return 0;
}

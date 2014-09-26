
#include <iostream>
#include <GL/glew.h>
#include "../include/Window.hpp"
#include "../include/Main.hpp"

Main::Main(int windowWidth, int windowHeight) :
window{windowWidth, windowHeight}
{
    init();
    mainLoop();
}

Main::~Main() {}

void Main::init()
{
    // Initialize Glew
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        std::cout << "Error: " << glewGetErrorString(res) << '\n';
        exit(1);
    }

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    //glPolygonMode(GL_FRONT, GL_LINE); // Wireframe mode

    window.init();
}

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

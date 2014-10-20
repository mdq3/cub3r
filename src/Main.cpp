/**
 * main.cpp
 *
 * @author mdq3
 */

#include "../include/Window.hpp"

int main(int argc, char* argv[])
{
    Window window(1200, 1000);
    while(window.isRunning())
    {
        window.handleEvents();
        window.renderScene();
    }
    window.close();
    return 0;
}

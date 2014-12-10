/*
  Copyright Michael Quested 2014.

  This file is part of Cub3r.

  Cub3r is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Cub3r is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Cub3r.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <SDL2/SDL_image.h>
#include "../include/Window.hpp"

Window::Window(int width, int height) :
width{width},
height{height},
running{true},
inFocus{true},
mouseSensitivity{0.005},
scene{width, height}
{
    initWindow();
    initGL();
    scene.initShadowMap(width, height);
    scene.initModels();
}

Window::~Window() {}

void Window::renderScene()
{
    scene.render();
    SDL_GL_SwapWindow(window);
}

void Window::initWindow()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << '\n';
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("Cub3r", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        std::cout << "Window could not be created! SDL Error:  " << SDL_GetError() << '\n';
        exit(1);
    }

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
        exit(1);
    }

    gContext = SDL_GL_CreateContext(window);
    if(gContext == NULL)
    {
        std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << '\n';
        exit(1);
    }
    SDL_GL_SetSwapInterval(1); // Sync buffer swap to screen refresh rate
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Window::initGL()
{
    glewExperimental = GL_TRUE;
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        std::cout << "Error: " << glewGetErrorString(res) << '\n';
        exit(1);
    }
    //std::cout << glGetString(GL_VERSION) << '\n';

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    //glPolygonMode(GL_FRONT, GL_LINE); // Wireframe mode
}

glm::vec3 Window::getCameraPosition()
{
    return scene.getCameraPosition();
}

void Window::close()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Window::isRunning()
{
    return running;
}

void Window::handleEvents()
{
    SDL_Event event;
    while(SDL_PollEvent(&event) != 0)
    {
        switch(event.type)
        {
        case SDL_QUIT:
            running = false;
            close();
            break;
        case SDL_KEYDOWN:
            handleKeyPressed(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            handleKeyReleased(event.key.keysym.sym);
            break;
        case SDL_MOUSEMOTION:
            scene.getCamera().pitch(event.motion.yrel * mouseSensitivity);
            scene.getCamera().yaw(-(event.motion.xrel * mouseSensitivity));
            break;
        case SDL_WINDOWEVENT:
            switch(event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                glViewport(0, 0, event.window.data1, event.window.data2);
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                inFocus = false;
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                inFocus = true;
                break;
            }
            break;
        }
    }
    scene.handleCamera();
}

void Window::handleKeyPressed(SDL_Keycode value)
{
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    switch(value)
    {
    case SDLK_ESCAPE:
    {
        running = false;
        break;
    }
    case SDLK_w:
    {
        scene.getCamera().setSpeedZ(scene.getCameraSpeed());
        break;
    }
    case SDLK_s:
    {
        scene.getCamera().setSpeedZ(-scene.getCameraSpeed());
        break;
    }
    case SDLK_a:
    {
        scene.getCamera().setSpeedX(scene.getCameraSpeed());
        break;
    }
    case SDLK_d:
    {
        scene.getCamera().setSpeedX(-scene.getCameraSpeed());
        break;
    }
    case SDLK_k:
    {
        if(keystates[SDL_SCANCODE_LSHIFT])

        {
            scene.getCube().rotateFront(90.0f, false);
        }
        else
        {
            scene.getCube().rotateFront(-90.0f, true);
        }
        break;
    }
    case SDLK_o:
    {
        if(keystates[SDL_SCANCODE_LSHIFT])
        {
            scene.getCube().rotateBack(-90.0f, false);
        }
        else
        {
            scene.getCube().rotateBack(90.0f, true);
        }
        break;
    }
    case SDLK_j:
    {
        if(keystates[SDL_SCANCODE_LSHIFT])
        {
            scene.getCube().rotateLeft(-90.0f, false);
        }
        else
        {
            scene.getCube().rotateLeft(90.0f, true);
        }
        break;
    }
    case SDLK_l:
    {
        if(keystates[SDL_SCANCODE_LSHIFT])
        {
            scene.getCube().rotateRight(90.0f, false);
        }
        else
        {
            scene.getCube().rotateRight(-90.0f, true);
        }
        break;
    }
    case SDLK_i:
    {
        if(keystates[SDL_SCANCODE_LSHIFT])
        {
            scene.getCube().rotateTop(90.0f, false);
        }
        else
        {
            scene.getCube().rotateTop(-90.0f, true);
        }
        break;
    }
    case SDLK_m:
    {
        if(keystates[SDL_SCANCODE_LSHIFT])
        {
            scene.getCube().rotateBottom(-90.0f, false);
        }
        else
        {
            scene.getCube().rotateBottom(90.0f, true);
        }
        break;
    }
    default:
        break;
    }
}

void Window::handleKeyReleased(SDL_Keycode value)
{
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    switch(value)
    {
    case SDLK_w:
    {
        if(!keystates[SDL_SCANCODE_S])
        {
            scene.getCamera().setSpeedZ(0.0f);
        }
        break;
    }
    case SDLK_s:
        {
        if(!keystates[SDL_SCANCODE_W])
        {
            scene.getCamera().setSpeedZ(0.0f);
        }
        break;
    }
    case SDLK_a:
    {
        if(!keystates[SDL_SCANCODE_D])
        {
            scene.getCamera().setSpeedX(0.0f);
        }
        break;
    }
    case SDLK_d:
    {
        if(!keystates[SDL_SCANCODE_A])
        {
            scene.getCamera().setSpeedX(0.0f);
        }
        break;
    }
    default:
        break;
    }
}
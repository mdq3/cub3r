
#include <iostream>
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
    window.display(); // End current frame (swaps front and back buffers)
}

void Window::initWindow()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    window.create(sf::VideoMode(width, height), "Cub3r", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);
    sf::Mouse::setPosition(sf::Vector2<int>(width/2, height/2), window);
    lastMousePos = sf::Mouse::getPosition(window);
}

void Window::initGL()
{
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        std::cout << "Error: " << glewGetErrorString(res) << '\n';
        exit(1);
    }

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
    window.close();
}

bool Window::isRunning()
{
    return running;
}

void Window::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            running = false;
            window.close();
            break;

        case sf::Event::Resized:
            glViewport(0, 0, event.size.width, event.size.height);
            width = event.size.width;
            height = event.size.height;
            break;

        case sf::Event::LostFocus:
            inFocus = false;
            break;

        case sf::Event::GainedFocus:
            inFocus = true;
            break;

        case sf::Event::KeyPressed:
            handleKeyPressed(event);
            break;

        case sf::Event::KeyReleased:
            handleKeyReleased(event);
            break;

        case sf::Event::MouseMoved:
            handleMouse(event);
            break;

        default:
            break;
        }
    }

    handleCamera();
}

void Window::handleCamera()
{
    scene.handleCamera();
    sf::Mouse::setPosition(sf::Vector2<int>(width/2, height/2), window);
    lastMousePos = sf::Mouse::getPosition(window);
}

void Window::handleKeyPressed(sf::Event event)
{
    switch(event.key.code)
    {
    case sf::Keyboard::Escape:
    {
        running = false;
        break;
    }

    case sf::Keyboard::W:
    {
        scene.getCamera().setSpeedZ(scene.getCameraSpeed());
        break;
    }

    case sf::Keyboard::S:
    {
        scene.getCamera().setSpeedZ(-scene.getCameraSpeed());
        break;
    }

    case sf::Keyboard::A:
    {
        scene.getCamera().setSpeedX(scene.getCameraSpeed());
        break;
    }

    case sf::Keyboard::D:
    {
        scene.getCamera().setSpeedX(-scene.getCameraSpeed());
        break;
    }

    case sf::Keyboard::K:
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            scene.getCube().rotateFront(90.0f, false);
        }
        else
        {
            scene.getCube().rotateFront(-90.0f, true);
        }
        break;
    }

    case sf::Keyboard::O:
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            scene.getCube().rotateBack(-90.0f, false);
        }
        else
        {
            scene.getCube().rotateBack(90.0f, true);
        }
        break;
    }

    case sf::Keyboard::J:
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            scene.getCube().rotateLeft(-90.0f, false);
        }
        else
        {
            scene.getCube().rotateLeft(90.0f, true);
        }
        break;
    }

    case sf::Keyboard::L:
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            scene.getCube().rotateRight(90.0f, false);
        }
        else
        {
            scene.getCube().rotateRight(-90.0f, true);
        }
        break;
    }

    case sf::Keyboard::I:
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            scene.getCube().rotateTop(90.0f, false);
        }
        else
        {
            scene.getCube().rotateTop(-90.0f, true);
        }
        break;
    }

    case sf::Keyboard::M:
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
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

void Window::handleKeyReleased(sf::Event event)
{
    switch(event.key.code)
    {
    case sf::Keyboard::W:
    {
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            scene.getCamera().setSpeedZ(0.0f);
        }
        break;
    }

    case sf::Keyboard::S:
        {
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            scene.getCamera().setSpeedZ(0.0f);
        }
        break;
    }

    case sf::Keyboard::A:
    {
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            scene.getCamera().setSpeedX(0.0f);
        }
        break;
    }

    case sf::Keyboard::D:
    {
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            scene.getCamera().setSpeedX(0.0f);
        }
        break;
    }

    default:
        break;
    }
}

void Window::handleMouse(sf::Event event)
{
    // TODO: if(inFocus) // check focus
    if(event.mouseMove.y - (height/2) > 0)
    {
        scene.getCamera().pitch((event.mouseMove.y - lastMousePos.y) * mouseSensitivity); // Down
    }
    else if(event.mouseMove.y - (height/2) < 0)
    {
        scene.getCamera().pitch((event.mouseMove.y - lastMousePos.y) * mouseSensitivity); // Up
    }
    if(event.mouseMove.x - (width/2) > 0)
    {
        scene.getCamera().yaw(-(event.mouseMove.x - lastMousePos.x) * mouseSensitivity);  // Right
    }
    else if(event.mouseMove.x - (width/2) < 0)
    {
        scene.getCamera().yaw(-(event.mouseMove.x - lastMousePos.x) * mouseSensitivity);  // Left
    }
}
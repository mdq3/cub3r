
#include "../include/Window.hpp"


Window::Window(int width, int height) :
windowWidth{width},
windowHeight{height},
running{true},
inFocus{true},
mouseSensitivity{0.005},
camera{windowWidth, windowHeight},
cameraSpeed{0.1}
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    window.create(sf::VideoMode(windowWidth, windowHeight), "Cub3r", sf::Style::Default, settings);

    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);
    sf::Mouse::setPosition(sf::Vector2<int>(windowWidth/2, windowHeight/2), window);

    sf::Mouse::setPosition(sf::Vector2<int>(windowWidth/2, windowHeight/2), window);
    lastMousePos = sf::Mouse::getPosition(window);
}

Window::~Window() {}

void Window::display()
{
    window.display();
}

void Window::close()
{
    window.close();
}

bool Window::isRunning()
{
    return running;
}

glm::mat4 Window::getProjectionViewMatrix()
{
    return camera.getProjectionViewMatrix();
}

glm::vec3 Window::getCameraPosition()
{
    return camera.getPosition();
}

void Window::handleEvents(Cube& cube)
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
            windowWidth = event.size.width;
            windowHeight = event.size.height;
            break;

        case sf::Event::LostFocus:
            inFocus = false;
            break;

        case sf::Event::GainedFocus:
            inFocus = true;
            break;

        case sf::Event::KeyPressed:
            handleKeyPressed(event, cube);
            break;

        case sf::Event::KeyReleased:
            handleKeyReleased(event, cube);
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
    camera.move();
    sf::Mouse::setPosition(sf::Vector2<int>(windowWidth/2, windowHeight/2), window);
    lastMousePos = sf::Mouse::getPosition(window);
}

void Window::handleKeyPressed(sf::Event event, Cube& cube)
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
        camera.setSpeedZ(cameraSpeed);
        break;
    }

    case sf::Keyboard::S:
    {
        camera.setSpeedZ(-cameraSpeed);
        break;
    }

    case sf::Keyboard::A:
    {
        camera.setSpeedX(cameraSpeed);
        break;
    }

    case sf::Keyboard::D:
    {
        camera.setSpeedX(-cameraSpeed);
        break;
    }

    case sf::Keyboard::K:
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            cube.rotateFrontAnticlockwise();
        }
        else
        {
            cube.rotateFrontClockwise();
        }
        break;
    }

    case sf::Keyboard::O:
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            cube.rotateBackAnticlockwise();
        }
        else
        {
            cube.rotateBackClockwise();
        }
        break;
    }

    case sf::Keyboard::J:
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            cube.rotateLeftAnticlockwise();
        }
        else
        {
            cube.rotateLeftClockwise();
        }
        break;
    }

    case sf::Keyboard::L:
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            cube.rotateRightAnticlockwise();
        }
        else
        {
            cube.rotateRightClockwise();
        }
        break;
    }

    case sf::Keyboard::I:
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            cube.rotateTopAnticlockwise();
        }
        else
        {
            cube.rotateTopClockwise();
        }
        break;
    }

    case sf::Keyboard::M:
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            cube.rotateBottomAnticlockwise();
        }
        else
        {
            cube.rotateBottomClockwise();
        }
        break;
    }

    default:
        break;
    }
}

void Window::handleKeyReleased(sf::Event event, Cube& cube)
{
    switch(event.key.code)
    {
    case sf::Keyboard::W:
    {
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            camera.setSpeedZ(0.0f);
        }
        break;
    }

    case sf::Keyboard::S:
        {
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            camera.setSpeedZ(0.0f);
        }
        break;
    }

    case sf::Keyboard::A:
    {
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            camera.setSpeedX(0.0f);
        }
        break;
    }

    case sf::Keyboard::D:
    {
        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            camera.setSpeedX(0.0f);
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
    if(event.mouseMove.y - (windowHeight/2) > 0)
    {
        camera.pitch((event.mouseMove.y - lastMousePos.y) * mouseSensitivity); // Down
    }
    else if(event.mouseMove.y - (windowHeight/2) < 0)
    {
        camera.pitch((event.mouseMove.y - lastMousePos.y) * mouseSensitivity); // Up
    }
    if(event.mouseMove.x - (windowWidth/2) > 0)
    {
        camera.yaw(-(event.mouseMove.x - lastMousePos.x) * mouseSensitivity);  // Right
    }
    else if(event.mouseMove.x - (windowWidth/2) < 0)
    {
        camera.yaw(-(event.mouseMove.x - lastMousePos.x) * mouseSensitivity);  // Left
    }
}
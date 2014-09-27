/**
 * Setup SFML window, handle events and 3D camera.
 *
 * @author mdq3
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include "Scene.hpp"

/**
 *
 */
class Window {
 public:
    /**
     * Constructor for Window. Create new SFML window with OpenGL context.
     *
     * @param width The initial width of the window
     * @param height The initial height of the window
     */
    Window(int width, int height);

    /**
     * Destructor for Window. Release resources.
     */
    ~Window();

    void renderScene();

    void initWindow();
    void initGL();

    glm::vec3 getCameraPosition();

    /**
     * Close the window.
     */
    void close();

    /**
     * Return the run state of the program.
     *
     * @return true if program is running, false if not
     */
    bool isRunning();

    /**
     * Handle window events.
     */
    void handleEvents();

    void handleCamera();

 private:
    sf::Window window;
    int windowWidth;
    int windowHeight;
    bool running;
    bool inFocus;
    sf::Vector2i lastMousePos;
    GLfloat mouseSensitivity;

    Scene scene;

    void handleKeyPressed(sf::Event event);

    void handleKeyReleased(sf::Event event);

    void handleMouse(sf::Event event);
};

#endif // WINDOW_H_
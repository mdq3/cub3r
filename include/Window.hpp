
#ifndef WINDOW_H_
#define WINDOW_H_


#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>

#include "Camera.hpp"
#include "Cube.hpp"


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

 	/**
 	 * Display the current frame. Swaps front and back buffers.
 	 */
 	void display();

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
 	 * Get the projection view matrix for the camera.
 	 *
 	 * @return the projection view matrix
 	 */
 	glm::mat4 getProjectionViewMatrix();

 	/**
 	 * Get the camera position.
 	 *
 	 * @return the camera position as a 3-dimensional vector
 	 */
 	glm::vec3 getCameraPosition();

 	/**
 	 * Handle window events.
 	 */
 	void handleEvents(Cube& cube);

 	/**
 	 * Handle the camera operations.
 	 */
 	void handleCamera();

 private:
 	sf::Window window;
	int windowWidth;
	int windowHeight;
	bool running;
	bool inFocus;
	sf::Vector2i lastMousePos;

	GLfloat mouseSensitivity;
	Camera camera;
	GLfloat cameraSpeed;

	void handleKeyPressed(sf::Event event, Cube& cube);

 	void handleKeyReleased(sf::Event event, Cube& cube);

 	void handleMouse(sf::Event event);
};

#endif // WINDOW_H_
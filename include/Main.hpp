
#ifndef MAIN_H_
#define MAIN_H_


#include <vector>
#include <GL/glew.h>

#include "Cube.hpp"
#include "Window.hpp"


/**
 *
 */
class Main {
 public:

	/**
	 * Constructor for a new Game. Creates the SFML window, initializes
	 * OpenGL, then invokes the main program loop.
	 *
	 * @param windowWidth The initial width of the window
	 * @param windowHeight The initial height of the window
	 */
	Main(int windowWidth, int windowHeight);

	/**
	 * Destructor for Game. Release resources.
	 */
	~Main();

 private:

 	Window window;
	GLuint currentShaderProgram;
	Cube cube;

	struct Light
	{
		glm::vec3 position;
		glm::vec3 rgb;
		GLfloat attenuation;
		GLfloat ambientCoefficient;
	} light;


	/**
	 * Create SFML window and initialize OpenGL.
	 */
	void init();

	/**
	 * Initialize models to be used in program.
	 */
	void initModels();

	/**
	 * Enter main program loop. Handles events and calls render() when
	 * necessary.
	 */
	void mainLoop();

	/**
	 * Draw the current frame.
	 */
	void renderScene();

	/**
	 * Send lights and camera data to the currently active shader.
	 */
	void updateLightsAndCamera();

	/**
	 * Reads shader source from file, compiles and attaches shader to
	 * shader program.
	 *
	 * @param shaderProgram The shader program to attach the compiled shader to
	 * @param file The path of the source file to be read
	 * @param shaderType The type of shader GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
	 */
	void addShader(GLuint shaderProgram, const char* file, GLenum shaderType);

	/**
	 * Create the shader program for use with this model.
	 *
	 * @param vertShaderPath The path of the source file for the vertex shader
	 * @param fragShaderPath The path of the source file for the fragment shader
	 */
	GLuint createShaderProgram(const char* vertShaderPath, const char* fragShaderPath);
};

#endif // MAIN_H_


#ifndef CUBE_H_
#define CUBE_H_


#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Model.hpp"
#include "Importer.hpp"


/**
 *
 */
class Cube {
 public:
 	/**
 	 * Constructor for Cube.
 	 */
	Cube();

	/**
 	 * Destructor for Cube. Release resources.
 	 */
	~Cube();

	/**
 	 * Create the cube by importing the data from file.
 	 */
	void setupCube(GLuint shaderProgram);

	/**
 	 * Render every mesh in the cube.
 	 */
	void render(glm::mat4 viewProjectionMatrix);

	void setRotation(GLfloat angle, glm::vec3 axis, GLfloat rate);

	void rotate();

	void operations();

 private:
 	struct Cub3
 	{
 		std::vector<Model> models;
 	};

 	std::vector<Cub3> cubes;

 	GLfloat slerpRate;
 	GLfloat currentSlerpVal;      // Current slerp value. If > 0, rotation in progress
 	GLfloat currentRotationAngle;
 	glm::vec3 currentRotationAxis;
};

#endif // CUBE_H_

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
     *
     * @param shaderProgram The shader program to use for the Cube
     */
    void setupCube(GLuint shaderProgram);

    /**
     * Render every mesh in the cube.
     *
     * @param viewProjectionMatrix The VPM matrix rendering the cube
     */
    void render(glm::mat4 viewProjectionMatrix);

    /**
     * Execute transformation operations for current render cycle.
     */
    void operations();

    void rotateFront(GLfloat angle);
    void rotateBack(GLfloat angle);
    void rotateLeft(GLfloat angle);
    void rotateRight(GLfloat angle);
    void rotateTop(GLfloat angle);
    void rotateBottom(GLfloat angle);

 private:
    std::vector<Model> cubes;

    bool rotate(GLfloat angle, glm::vec3 axis, std::vector<int> faces);
};

#endif // CUBE_H_
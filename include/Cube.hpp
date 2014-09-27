/**
 * Representation of 3D rotating cube puzzle.
 *
 * @author mdq3
 */

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
    Cube(GLuint shaderProgram);

    /**
     * Destructor for Cube. Release resources.
     */
    ~Cube();

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

    void rotateFront(GLfloat angle, bool clockwise);
    void rotateBack(GLfloat angle, bool clockwise);
    void rotateLeft(GLfloat angle, bool clockwise);
    void rotateRight(GLfloat angle, bool clockwise);
    void rotateTop(GLfloat angle, bool clockwise);
    void rotateBottom(GLfloat angle, bool clockwise);

 private:
    std::vector<Model> cubes; // Cube models which make up the whole cube puzzle

    // Indices for each cube for each face in cubes
    std::vector<int> frontFace  = {6, 7, 3, 2, 12, 11, 9, 10, 22};
    std::vector<int> backFace   = {5, 4, 0, 1, 17, 13, 8, 15, 20};
    std::vector<int> leftFace   = {4, 6, 2, 0, 18, 10, 14, 13, 23};
    std::vector<int> rightFace  = {7, 5, 1, 3, 19, 15, 16, 11, 24};
    std::vector<int> topFace    = {4, 5, 7, 6, 17, 19, 12, 18, 25};
    std::vector<int> bottomFace = {2, 3, 1, 0, 9, 16, 8, 14, 21};

    /**
     *
     */
    void rotate(GLfloat angle, glm::vec3 axis, std::vector<int> face, bool clockwise);
};

#endif // CUBE_H_
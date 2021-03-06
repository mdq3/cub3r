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
    Cube();

    /**
     * Constructor for Cube.
     */
    Cube(GLuint shaderProgram);

    /**
     * Destructor for Cube. Release resources.
     */
    ~Cube();

    void renderShadowMap(GLuint shadowProgram);

    /**
     * Render every mesh in the cube.
     *
     * @param viewProjectionMatrix The VPM matrix rendering the cube
     */
    void render(glm::mat4 viewProjectionMatrix, glm::mat4 depthBiasMVP);

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
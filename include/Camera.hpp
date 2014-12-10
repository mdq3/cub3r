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
 * 3D camera.
 *
 * @author mdq3
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

/**
 *
 */
class Camera {
 public:
    Camera(int windowWidth, int windowHeight);

    ~Camera();

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::mat4 getProjectionViewMatrix();
    glm::vec3 getPosition();

    void setSpeedX(GLfloat speed);
    void setSpeedY(GLfloat speed);
    void setSpeedZ(GLfloat speed);

    void move();
    void pitch(GLfloat angle);
    void yaw(GLfloat angle);
    void roll(GLfloat angle);

    void setProjectionPerspective(GLfloat fov, GLfloat aspectRatio, GLfloat near, GLfloat far);
    void setProjectionOrthographic();

    /**
     * Print camera vectors for testing purposes.
     */
    void printInfo();

 private:

    // LookAt vectors
    glm::vec3 eyePos;
    glm::vec3 center;
    glm::vec3 up;

    glm::vec3 direction; // Forward/Backward direction
    glm::vec3 sideways;  // Right/Left direction

    glm::mat4 projectionMatrix;

    GLfloat currentSpeedX;
    GLfloat currentSpeedY;
    GLfloat currentSpeedZ;
};

#endif // CAMERA_H_

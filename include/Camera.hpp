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

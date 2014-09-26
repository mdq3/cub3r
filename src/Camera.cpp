
#include <iostream>
#include <iomanip> // For printInfo()
#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/Camera.hpp"

Camera::Camera(int WindowWidth, int WindowHeight) :
//eyePos(glm::vec3(0.0f, 0.0f, 10.0f)),
//center(glm::vec3(0.0f, 0.0f, 0.0f)),
//up(glm::vec3(0.0f, 1.0f, 0.0f)),
//direction(glm::vec3(0.0f, 0.0f, -1.0f)),
//sideways(glm::vec3(-1.0f, 0.0f, 0.0f)),

eyePos(glm::vec3(8.96f, 7.22f, 9.77f)),
center(glm::vec3(8.33f, 6.73f, 9.16f)),
up(glm::vec3(0.0f, 1.0f, 0.0f)),
direction(glm::vec3(-0.63f, -0.49f, -0.61f)),
sideways(glm::vec3(-0.70f, 0.00f, 0.72f)),
currentSpeedX{0.0f}, currentSpeedY{0.0f}, currentSpeedZ{0.0f}
{
    setProjectionPerspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);
}

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix()
{
    glm::mat4 ViewMatrix = glm::lookAt(
        eyePos,
        center,
        up
    );
    return ViewMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
    return projectionMatrix;
}

glm::mat4 Camera::getProjectionViewMatrix()
{
    return projectionMatrix * getViewMatrix();
}

glm::vec3 Camera::getPosition()
{
    return eyePos;
}

void Camera::setSpeedX(GLfloat speed)
{
    currentSpeedX = speed;
}

void Camera::setSpeedY(GLfloat speed)
{
    currentSpeedY = speed;
}

void Camera::setSpeedZ(GLfloat speed)
{
    currentSpeedZ = speed;
}

void Camera::move()
{
    eyePos += (direction * currentSpeedZ) + (sideways * currentSpeedX);
    center = eyePos + direction;
}

void Camera::setProjectionPerspective(GLfloat fov, GLfloat aspectRatio, GLfloat near, GLfloat far)
{
    projectionMatrix = glm::perspective(fov, aspectRatio, near, far);
}

void Camera::setProjectionOrthographic()
{
    projectionMatrix = glm::ortho(
        -1.0f,    // Left
         1.0f,    // Right
        -1.0f,    // Bottom
         1.0f,    // Top
         0.1f,    // zear
         100.0f); // zFar
}

void Camera::roll(GLfloat angle)
{

}

void Camera::pitch(GLfloat angle)
{
    if((angle < 0 && direction[1] <= 0.99f) || (angle > 0 && direction[1] >= -0.99f))
    {
        direction = glm::rotate(direction, angle, sideways);
        center = eyePos + direction;
    }
}

void Camera::yaw(GLfloat angle)
{
    direction = glm::rotate(direction, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    sideways = glm::rotate(sideways, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    center = eyePos + direction;
}


void Camera::printInfo()
{
    std::cout << std::fixed << std::setprecision(2) << "Direction: " << direction[0] << ' ' << direction[1] << ' ' << direction[2] << '\n';
    std::cout << std::fixed << std::setprecision(2) << "Position:  " << eyePos[0]    << ' ' << eyePos[1]    << ' ' << eyePos[2]    << '\n';
    std::cout << std::fixed << std::setprecision(2) << "Target:    " << center[0]    << ' ' << center[1]    << ' ' << center[2]    << '\n';
    std::cout << std::fixed << std::setprecision(2) << "Up:        " << up[0]        << ' ' << up[1]        << ' ' << up[2]        << '\n';
    std::cout << std::fixed << std::setprecision(2) << "Sideways:  " << sideways[0]  << ' ' << sideways[1]  << ' ' << sideways[2]  << '\n';
    std::cout << '\n';
}




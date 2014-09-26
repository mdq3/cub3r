/**
 * Represents a 3D scene with lights, camera and models.
 *
 * @author mdq3
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Camera.hpp"
#include "Cube.hpp"

class Scene {
 public:

    Scene(int width, int height);

    ~Scene();

    void render();

    void initModels();

    Cube& getCube();
    Camera& getCamera();
    GLfloat getCameraSpeed();

    /**
     * Get the camera position.
     *
     * @return the camera position as a 3-dimensional vector
     */
    glm::vec3 getCameraPosition();

    /**
     * Get the projection view matrix for the camera.
     *
     * @return the projection view matrix
     */
    glm::mat4 getProjectionViewMatrix();

    /**
     * Handle the camera operations.
     */
    void handleCamera();

    /**
     * Send lights and camera data to the currently active shader.
     */
    void updateLightsAndCamera();

 private:
    Camera camera;
    GLfloat cameraSpeed;
    Cube cube;
    GLuint currentShaderProgram;

    struct Light
    {
        glm::vec3 position;
        glm::vec3 rgb;
        GLfloat attenuation;
        GLfloat ambientCoefficient;
    } light;

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


#endif // SCENE_H_
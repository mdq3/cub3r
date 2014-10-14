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
#include "Model.hpp"

class Scene {
 public:

    Scene(int width, int height);

    ~Scene();

    void render();

    void renderPass();
    void shadowPass();

    void initModels();

    void initShadowMap(int windowWidth, int windowHeight);

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
    std::vector<Cube> assets;
    GLuint currentShaderProgram;

    GLuint FBOshadow; // Framebuffer object for shadow mapping
    GLuint shadowMap; // The texture for shadow mapping
    glm::mat4 depthBiasMVP;
    GLuint shaderProgramShadowMap;

    Model plane;

    // Bias matrix for converting space coord to image coord (-1.0 to 1.0 -> 0.0 to 1.0)
    const glm::mat4 biasMatrix = glm::mat4(
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f);

    struct Light
    {
        glm::vec3 position;
        glm::vec3 rgb;
        GLfloat attenuation;
        GLfloat ambientCoefficient;
        glm::vec3 target;
        glm::vec3 direction;
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

    void visualizeShadowMap();
};


#endif // SCENE_H_
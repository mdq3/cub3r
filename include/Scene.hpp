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

#include <memory>
#include "Light.hpp"

class Scene {
 public:

    Scene(int width, int height);

    ~Scene();

    /**
     * Initialize the models that appear in this scene.
     */
    void initModels();

    /**
     * Initialize the shadow map to be used in this scene.
     */
    void initShadowMap(int windowWidth, int windowHeight);

    /**
     * Render the entire scene.
     */
    void render();

    /**
     * Render the scene from light's POV into depth frame buffer.
     */
    void shadowPass();

    /**
     * Render all of the scene's models using camera, lighting and
     * shadow data.
     */
    void renderPass();

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
    Model plane;
    GLuint currentShaderProgram;

    GLuint FBOshadow;              // Framebuffer object for shadow mapping
    GLuint shadowMap;              // The texture for shadow mapping
    glm::mat4 depthBiasMVP;        // The view projection matrix in light-space for shadow map rendering
    GLuint shaderProgramShadowMap; // Shader program for rendering shadow map

    std::vector<std::unique_ptr<Lighter>> lights;

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
        glm::vec3 target;
        glm::vec3 direction;
    } light;

    GLfloat ambientLightValue;

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

#include <iostream>
#include <fstream>
#include <string>
#include "../include/Scene.hpp"

Scene::Scene(int width, int height) :
camera{width, height},
cameraSpeed{0.1}
{
    light.position = glm::vec3(0.0f, 0.0f, 5.0f);
    light.rgb = glm::vec3(1.0f, 1.0f, 1.0f);
    light.attenuation = 0.0f;
    light.ambientCoefficient = 0.5f;
}

Scene::~Scene() {}

void Scene::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0); // Vertex position attribute
    glEnableVertexAttribArray(1); // Vertex normal attribute
    glEnableVertexAttribArray(2); // Vertex UV coordinate attribute

    updateLightsAndCamera();
    cube.operations();
    cube.render(getProjectionViewMatrix());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Scene::initModels()
{
    const char* vert = "resources/shaders/shader.vert";
    const char* frag = "resources/shaders/shader.frag";
    GLuint shaderProgram = createShaderProgram(vert, frag);

    glUseProgram(shaderProgram);
    currentShaderProgram = shaderProgram;

    cube.setupCube(shaderProgram);
}

Cube& Scene::getCube()
{
    return cube;
}

Camera& Scene::getCamera()
{
    return camera;
}

GLfloat Scene::getCameraSpeed()
{
    return cameraSpeed;
}

glm::vec3 Scene::getCameraPosition()
{
    return camera.getPosition();
}

glm::mat4 Scene::getProjectionViewMatrix()
{
    return camera.getProjectionViewMatrix();
}

void Scene::handleCamera()
{
    camera.move();
}

void Scene::updateLightsAndCamera()
{
    GLint lightLocation = glGetUniformLocation(currentShaderProgram, "light.position");
    glUniform3fv(lightLocation, 1, &light.position[0]);

    GLint lightRGB = glGetUniformLocation(currentShaderProgram, "light.rgb");
    glUniform3fv(lightRGB, 1, &light.rgb[0]);

    GLint attenuation = glGetUniformLocation(currentShaderProgram, "light.attenuation");
    glUniform1f(attenuation, light.attenuation);

    GLint ambientCoefficient = glGetUniformLocation(currentShaderProgram, "light.ambientCoefficient");
    glUniform1f(ambientCoefficient, light.ambientCoefficient);

    GLint cameraPosition = glGetUniformLocation(currentShaderProgram, "cameraPosition");
    glUniform3fv(cameraPosition, 1, &getCameraPosition()[0]);
}

void Scene::addShader(GLuint shaderProgram, const char* file, GLenum shaderType)
{
    std::string shaderString;
    const GLchar* shaderSrc;
    std::ifstream sourceFile(file);

    if(sourceFile)
    {
        shaderString.assign((std::istreambuf_iterator<char>(sourceFile)), std::istreambuf_iterator<char>());
        shaderSrc = shaderString.c_str();
    }
    else
    {
        std::cout << "Unable to open shader source file: " << file << '\n';
        exit(1);
    }

    GLuint shaderObj = glCreateShader(shaderType);
    if(shaderObj == 0)
    {
        std::cout << "Error creating shader\n";
        exit(1);
    }

    glShaderSource(shaderObj, 1, (const GLchar**)&shaderSrc, NULL);
    glCompileShader(shaderObj);
    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
        std::cout << "Error compiling shader: " << infoLog << '\n';
        exit(1);
    }

    glAttachShader(shaderProgram, shaderObj);
}

GLuint Scene::createShaderProgram(const char* vertShaderPath, const char* fragShaderPath)
{
    GLuint shaderProgram = glCreateProgram();

    if(shaderProgram == 0)
    {
        std::cout << "Error creating shader program\n";
        exit(1);
    }

    addShader(shaderProgram, vertShaderPath, GL_VERTEX_SHADER);
    addShader(shaderProgram, fragShaderPath, GL_FRAGMENT_SHADER);

    GLint success = 0;
    GLchar errorLog[1024] = {0};

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(success == 0)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        std::cout << "Error linking shader program: " << errorLog << '\n';
        exit(1);
    }

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        std::cout << "Invalid shader program: " << errorLog << '\n';
        exit(1);
    }
    return shaderProgram;
}


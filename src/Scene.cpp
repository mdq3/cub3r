
#include <iostream>
#include <fstream>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "../include/Scene.hpp"

Scene::Scene(int width, int height) :
camera{width, height},
cameraSpeed{0.1},
depthBiasMVP(glm::mat4(1.0f))
{
    light.position = glm::vec3(-10.0f, 10.0f, 10.0f);
    light.rgb = glm::vec3(1.0f);
    light.attenuation = 0.0f;
    light.ambientCoefficient = 0.5f;
    light.target = glm::vec3(0.0f, 0.0f, 0.0f);
    light.direction = glm::normalize(glm::vec3(light.position.x - light.target.x,
                                               light.position.y - light.target.y,
                                               light.position.z - light.target.z));
}

Scene::~Scene() {}

void Scene::initModels()
{
    currentShaderProgram = createShaderProgram("resources/shaders/shader.vert",
                                               "resources/shaders/shader.frag");
    Cube c(currentShaderProgram);
    cube = c;

    Importer importer("resources/models/plane.q3d");
    std::vector<Importer::Mesh> objects = importer.getObjects();
    std::vector<glm::vec3> vs = objects[0].vs;
    std::vector<glm::vec3> ns = objects[0].ns;
    std::vector<glm::vec2> uv = objects[0].uvs;
    std::string texture       = objects[0].texturePath;
    GLuint vertexCount        = objects[0].vsSize;

    Model model(vs, ns, uv, texture, vertexCount, currentShaderProgram, 0.0f, true);
    plane = model;
}

void Scene::initShadowMap(int windowWidth, int windowHeight)
{
    shaderProgramShadowMap = createShaderProgram("resources/shaders/shadow_map.vert",
                                                 "resources/shaders/shadow_map.frag");
    glGenFramebuffers(1, &FBOshadow);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOshadow);
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Depth framebuffer failed to initialize.\n";
        exit(1);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Bind windowing system's framebuffer
}

void Scene::render()
{
    shadowPass();
    //visualizeShadowMap(); // For testing purposes only
    renderPass();
}

void Scene::shadowPass()
{
    glm::mat4 depthProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 10.0f, 20.0f);
    glm::mat4 depthViewMatrix = glm::lookAt(light.position, light.target, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 shadowViewProjection = depthProjectionMatrix * depthViewMatrix;
    depthBiasMVP = biasMatrix * depthProjectionMatrix * depthViewMatrix;

    glUseProgram(shaderProgramShadowMap);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOshadow);
    glClear(GL_DEPTH_BUFFER_BIT);
    glPolygonOffset(4.0f, 4.0f);
    glEnable(GL_POLYGON_OFFSET_FILL); // Avoid depth fighting issues

    GLint mvp = glGetUniformLocation(shaderProgramShadowMap, "shadowViewProjectionMatrix");
    glUniformMatrix4fv(mvp, 1, GL_FALSE, &shadowViewProjection[0][0]);

    glEnableVertexAttribArray(0);

    cube.operations();
    cube.renderShadowMap(shaderProgramShadowMap);
    plane.renderShadowMap(shaderProgramShadowMap);

    glDisableVertexAttribArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Bind windowing system's framebuffer
    glDisable(GL_POLYGON_OFFSET_FILL);
}

void Scene::renderPass()
{
    glUseProgram(currentShaderProgram);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateLightsAndCamera();

    glEnableVertexAttribArray(0); // Vertex position attribute
    glEnableVertexAttribArray(1); // Vertex normal attribute
    glEnableVertexAttribArray(2); // Vertex UV coordinate attribute

    GLint shadowMatrixLocation = glGetUniformLocation(currentShaderProgram, "depthBiasMVP");
    glUniformMatrix4fv(shadowMatrixLocation, 1, GL_FALSE, &depthBiasMVP[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    GLint shadowMapID = glGetUniformLocation(currentShaderProgram, "shadowMap");
    glUniform1i(shadowMapID, 0);

    cube.render(getProjectionViewMatrix(), depthBiasMVP);
    plane.render(getProjectionViewMatrix(), depthBiasMVP);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
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
    // TODO: Only update if changed.

    GLint lightLocation = glGetUniformLocation(currentShaderProgram, "light.position");
    glUniform3fv(lightLocation, 1, &light.position[0]);

    GLint lightRGB = glGetUniformLocation(currentShaderProgram, "light.rgb");
    glUniform3fv(lightRGB, 1, &light.rgb[0]);

    GLint attenuation = glGetUniformLocation(currentShaderProgram, "light.attenuation");
    glUniform1f(attenuation, light.attenuation);

    GLint ambientCoefficient = glGetUniformLocation(currentShaderProgram, "light.ambientCoefficient");
    glUniform1f(ambientCoefficient, light.ambientCoefficient);

    GLint direction = glGetUniformLocation(currentShaderProgram, "light.direction");
    glUniform3fv(direction, 1, &light.direction[0]);

    GLint isPoint = glGetUniformLocation(currentShaderProgram, "light.isPoint");
    glUniform1i(isPoint, GL_FALSE);

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

void Scene::visualizeShadowMap()
{
    const GLfloat quadData[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
    };
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    GLuint VBOquad;
    glGenBuffers(1, &VBOquad);
    glBindBuffer(GL_ARRAY_BUFFER, VBOquad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadData), quadData, GL_STATIC_DRAW);

    GLuint quadProgram = createShaderProgram("resources/shaders/shadtest.vert",
                                             "resources/shaders/shadtest.frag");

    glUseProgram(quadProgram);
    GLuint texID = glGetUniformLocation(quadProgram, "textureSampler");
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glUniform1i(texID, 2);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBOquad);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
}

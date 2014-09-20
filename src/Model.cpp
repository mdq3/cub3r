
#include <iostream>
#include <fstream>
#include <math.h>

#include <SFML/Graphics.hpp>

#include "../include/Model.hpp"


Model::Model(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs,
             std::string texturePath,GLuint vCount, GLuint shader, bool dynamicDraw) :
shaderProgram{shader},
vertexCount{vCount},
materialShininess{50.0f},
materialSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f))
{
    GLenum usage = GL_STATIC_DRAW;
    if(dynamicDraw)
    {
        usage = GL_DYNAMIC_DRAW;
    }

    createVBO(VBOposition, vertices, usage);
    createVBO(VBOnormal, normals, usage);
    createUVBuffer(VBOuv, uvs, usage);
    createTexture(texturePath);
}

Model::~Model() {}

void Model::scale(glm::vec3 amount)
{
    modelScaleMatrix = glm::scale(modelScaleMatrix, amount);
}

void Model::translate(glm::vec3 amount)
{
    modelTranslateMatrix = glm::translate(modelTranslateMatrix, amount);
}

void Model::localRotate(GLfloat angle, glm::vec3 axis, GLfloat slerp)
{
    rotate(modelLocalRotateMatrix, angle, axis, slerp);
}

void Model::worldRotate(GLfloat angle, glm::vec3 axis, GLfloat slerp)
{
    rotate(modelWorldRotateMatrix, angle, axis, slerp);
}

void Model::render(glm::mat4 viewProjectionMatrix)
{
    modelMatrix = modelScaleMatrix *
                  modelWorldRotateMatrix *
                  modelTranslateMatrix *
                  modelLocalRotateMatrix;

    // Send MVP matrix to shader
    GLint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "modelViewProjectionMatrix"); // Get the location of the MVP matrix in the shader
    glm::mat4 modelViewProjectionMatrix = viewProjectionMatrix * modelMatrix;                          // Create the new MVP matrix
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);       // Send the MVP matrix to the shader

    // Send model transform matrix to shader
    GLint modelMatrixLocation = glGetUniformLocation(shaderProgram, "modelTransformMatrix");
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    // Send material properties to shader
    GLint shininess = glGetUniformLocation(shaderProgram, "materialShininess");
    glUniform1f(shininess, materialShininess);

    GLint specularColor = glGetUniformLocation(shaderProgram, "materialSpecularColor");
    glUniform3fv(specularColor, 1, &materialSpecularColor[0]);

    // Vertex buffer processing
    glBindBuffer(GL_ARRAY_BUFFER, VBOposition);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOnormal);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindBuffer(GL_ARRAY_BUFFER, VBOuv);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Model::operations()
{
    if(currentSlerpVal > 0.0f)
    {
        doRotation();
    }
}

void Model::createVBO(GLuint& VBO, std::vector<glm::vec3> data, GLenum usage)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec3), &data[0], usage);
}

void Model::createUVBuffer(GLuint& VBOuv, std::vector<glm::vec2> data, GLenum usage)
{
    glGenBuffers(1, &VBOuv);
    glBindBuffer(GL_ARRAY_BUFFER, VBOuv);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec2), &data[0], usage);
}

void Model::createTexture(std::string filePath)
{
    sf::Image image;
    if (!image.loadFromFile(filePath))
    {
        std::cout << "Error: could not load texture " << filePath;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Model::generateVertexNormals(std::vector<glm::vec3> vertices, std::vector<GLuint> indices, GLenum usage)
{
    std::vector<glm::vec3> normals;
    for(GLuint i = 0; i < vertices.size(); ++i)
    {
        normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    for(GLuint i = 0; i < indices.size() / 3; ++i)
    {
        glm::vec3 firstVec  = vertices[indices[i * 3]] - vertices[indices[i * 3 + 1]];
        glm::vec3 secondVec = vertices[indices[i * 3]] - vertices[indices[i * 3 + 2]];
        glm::vec3 faceNormal = glm::normalize(glm::cross(firstVec, secondVec));

        normals[indices[i * 3]] += faceNormal;
        normals[indices[i * 3 + 1]] += faceNormal;
        normals[indices[i * 3 + 2]] += faceNormal;
    }
    createVBO(VBOnormal, normals, usage);
}

void Model::rotate(glm::mat4& rotateMatrix, GLfloat angle, glm::vec3 axis, GLfloat slerp)
{
    if(currentSlerpVal == 0.0f)
    {
        currentRotationAngle = angle;
        currentRotationAxis = axis;
        slerpRate = slerp;
        currentSlerpVal += slerpRate;
    }

    angle *= M_PI / 180.0f; // Convert degrees into radians
    glm::quat target = glm::normalize(glm::angleAxis(angle, axis)) * glm::quat_cast(currentModelWorldRotateMatrix);
    glm::quat mix = glm::mix(glm::quat_cast(currentModelWorldRotateMatrix), target, slerp);
    rotateMatrix = glm::mat4_cast(mix);

    if(currentSlerpVal == 1.0f)
    {
        currentModelWorldRotateMatrix = rotateMatrix;
        //currentModelWorldRotateMatrix = glm::rotate(currentModelWorldRotateMatrix, angle, axis);
    }
}

void Model::doRotation()
{
    localRotate(currentRotationAngle, currentRotationAxis, currentSlerpVal);
    currentSlerpVal += slerpRate;
    currentSlerpVal = floor(currentSlerpVal * pow(10.0f, 2) + 0.5f) / pow(10.0f, 2); // Correct float arith errors
    if(currentSlerpVal > 1.0f)
    {
        currentSlerpVal = 0.0f;
    }
}

bool Model::isRotating()
{
    if(currentSlerpVal > 0.0f)
    {
        return true;
    }
    return false;
}

#include <iostream>
#include <fstream>
#include <math.h>

#include "../include/Model.hpp"


Model::Model(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<GLuint>& indices,
             std::vector<glm::vec3> colors, GLuint iCount, GLuint shader, bool dynamicDraw) :
indexCount{iCount},
materialShininess{50.0f},
materialSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f)),
shaderProgram{shader}
{
    GLenum usage = GL_STATIC_DRAW;
    if(dynamicDraw)
    {
        usage = GL_DYNAMIC_DRAW;
    }

    createVBO(VBOposition, vertices, usage);
    createVBO(VBOcolor, colors, usage);
    createVBO(VBOnormal, normals, usage);
    createIBO(indices, usage);
}

Model::~Model()
{
    // Release resources
}

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

GLuint Model::getShaderProgram()
{
    return shaderProgram;
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

    glBindBuffer(GL_ARRAY_BUFFER, VBOcolor);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOnormal);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Model::createVBO(GLuint& VBO, std::vector<glm::vec3> data, GLenum usage)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec3), &data[0], usage);
}

void Model::createIBO(std::vector<GLuint> data, GLenum usage)
{
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(GLuint), &data[0], usage);
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
    angle *= M_PI / 180.0f; // Convert degrees into radians
    glm::quat target = glm::quat_cast(currentModelWorldRotateMatrix) * glm::angleAxis(angle, axis);
    glm::quat mix = glm::mix(glm::quat_cast(currentModelWorldRotateMatrix), target, slerp);
    rotateMatrix = glm::mat4_cast(mix);

    if(slerp == 1.0f)
    {
        currentModelWorldRotateMatrix = modelWorldRotateMatrix;
    }
}
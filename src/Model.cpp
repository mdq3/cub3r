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

#include <iostream>
#include <fstream>
#include <math.h>
#include <SDL2/SDL_image.h>
#include "../include/Model.hpp"

Model::Model() {}

Model::Model(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs,
             std::string texturePath, GLuint vCount, GLuint shader, GLfloat shininess, bool dynamicDraw) :
shaderProgram{shader},
vertexCount{vCount},
materialShininess{shininess}
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

void Model::renderShadowMap(GLuint shadowProgram)
{
    //glViewport(0, 0, 1200, 1000);

    GLint modelMatrixLocation = glGetUniformLocation(shadowProgram, "modelMatrix");
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOposition);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Model::render(glm::mat4 viewProjectionMatrix, glm::mat4 depthBiasMVP)
{
    GLint mvpMatrixLocation = glGetUniformLocation(shaderProgram, "modelViewProjectionMatrix");
    glm::mat4 modelViewProjectionMatrix = viewProjectionMatrix * modelMatrix;
    glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);

    GLint modelMatrixLocation = glGetUniformLocation(shaderProgram, "modelTransformMatrix");
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    glm::mat3 normalMatrix = glm::inverse(glm::mat3(modelMatrix));
    GLint normalMatrixLocation = glGetUniformLocation(shaderProgram, "normalMatrix");
    glUniformMatrix3fv(normalMatrixLocation, 1, GL_TRUE, &normalMatrix[0][0]);

    GLint shininess = glGetUniformLocation(shaderProgram, "materialShininess");
    glUniform1f(shininess, materialShininess);

    glBindBuffer(GL_ARRAY_BUFFER, VBOposition);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOnormal);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint textureID = glGetUniformLocation(shaderProgram, "textureSampler");
    glUniform1i(textureID, 1);
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
    modelMatrix = modelScaleMatrix * modelWorldRotateMatrix * modelTranslateMatrix * modelLocalRotateMatrix;
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
    SDL_Surface* image = IMG_Load(filePath.c_str());
    if(image == NULL)
    {
        std::cout << "Could not load image! SDL_image Error: " << filePath.c_str() << ' ' << IMG_GetError() << '\n';
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image->w, image->h,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    SDL_FreeSurface(image);
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
    // Correct float arith errors to 3 decimal places
    currentSlerpVal = floor(currentSlerpVal * pow(10.0f, 3) + 0.5f) / pow(10.0f, 3);
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
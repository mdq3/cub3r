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
#include <cmath>
#include "../include/Cube.hpp"

Cube::Cube() {}

Cube::Cube(GLuint shaderProgram)
{
    Importer importer("resources/models/cub3.q3d");
    std::vector<Importer::Mesh> objects = importer.getObjects();
    for(Importer::Mesh& mesh : objects)
    {
            std::vector<glm::vec3> vs = mesh.vs;
            std::vector<glm::vec3> ns = mesh.ns;
            std::vector<glm::vec2> uv = mesh.uvs;
            std::string texture       = mesh.texturePath;
            GLuint vertexCount        = mesh.vsSize;

            Model model(vs, ns, uv, texture, vertexCount, shaderProgram, 50.0f, true);
            cubes.push_back(model);
    }
}

Cube::~Cube() {}

void Cube::renderShadowMap(GLuint shadowProgram)
{
    for(Model& cube : cubes)
    {
        cube.renderShadowMap(shadowProgram);
    }
}

void Cube::render(glm::mat4 viewProjectionMatrix, glm::mat4 depthBiasMVP)
{
    for(Model& cube : cubes)
    {
        cube.render(viewProjectionMatrix, depthBiasMVP);
    }
}

void Cube::operations()
{
    for(Model& cube : cubes)
    {
        cube.operations();
    }
}

void Cube::rotateFront(GLfloat angle, bool clockwise)
{
    rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f), frontFace, clockwise);
}

void Cube::rotateBack(GLfloat angle, bool clockwise)
{
    rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f), backFace, clockwise);
}

void Cube::rotateLeft(GLfloat angle, bool clockwise)
{
    rotate(angle, glm::vec3(1.0f, 0.0f,0.0f), leftFace, clockwise);
}

void Cube::rotateRight(GLfloat angle, bool clockwise)
{
    rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f), rightFace, clockwise);
}

void Cube::rotateTop(GLfloat angle, bool clockwise)
{
    rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f), topFace, clockwise);
}

void Cube::rotateBottom(GLfloat angle, bool clockwise)
{
    rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f), bottomFace, clockwise);
}

void Cube::rotate(GLfloat angle, glm::vec3 axis, std::vector<int> face, bool clockwise)
{
    bool rotating = false;
    for(Model& cube : cubes)
    {
        if(cube.isRotating())
        {
            rotating = true;
        }
    }

    if(!rotating)
    {
        for(unsigned int i = 0; i < face.size(); ++i)
        {
            cubes[face[i]].localRotate(angle, axis, 0.05f);
        }

        if(clockwise)
        {
            Model temp = cubes[face[0]];
            cubes[face[0]] = cubes[face[3]];
            cubes[face[3]] = cubes[face[2]];
            cubes[face[2]] = cubes[face[1]];
            cubes[face[1]] = temp;
            temp = cubes[face[4]];
            cubes[face[4]] = cubes[face[7]];
            cubes[face[7]] = cubes[face[6]];
            cubes[face[6]] = cubes[face[5]];
            cubes[face[5]] = temp;
        }
        else
        {
            Model temp = cubes[face[0]];
            cubes[face[0]] = cubes[face[1]];
            cubes[face[1]] = cubes[face[2]];
            cubes[face[2]] = cubes[face[3]];
            cubes[face[3]] = temp;
            temp = cubes[face[4]];
            cubes[face[4]] = cubes[face[5]];
            cubes[face[5]] = cubes[face[6]];
            cubes[face[6]] = cubes[face[7]];
            cubes[face[7]] = temp;
        }
    }
}
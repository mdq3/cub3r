
#include <iostream>
#include <cmath>
#include "../include/Cube.hpp"

Cube::Cube() {}

Cube::~Cube() {}

void Cube::setupCube(GLuint shaderProgram)
{
    Importer importer("resources/models/cub3.q3d");

    std::vector<Importer::Mesh> objects = importer.getObjects();
    for(Importer::Mesh& mesh : objects)
    {
            std::vector<glm::vec3> vs = mesh.vs;
            std::vector<glm::vec3> ns = mesh.ns;
            std::vector<glm::vec2> uv = mesh.uvs;
            std::string texture       = mesh.texturePath;

            GLuint vertexCount = mesh.vsSize;

            Model model(vs, ns, uv, texture, vertexCount, shaderProgram, true);
            cubes.push_back(model);
    }
}

void Cube::render(glm::mat4 viewProjectionMatrix)
{
    for(Model& cube : cubes)
    {
        cube.render(viewProjectionMatrix);
    }
}

void Cube::operations()
{
    for(Model& cube : cubes)
    {
        cube.operations();
    }
}

void Cube::rotateFrontClockwise()
{
    rotate(-90.0f, glm::vec3(0.0f, 0.0f, 1.0f), frontFace, true);
}

void Cube::rotateBackClockwise()
{
    rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f), backFace, true);
}

void Cube::rotateLeftClockwise()
{
    rotate(90.0f, glm::vec3(1.0f, 0.0f,0.0f), leftFace, true);
}

void Cube::rotateRightClockwise()
{
    rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f), rightFace, true);
}

void Cube::rotateTopClockwise()
{
    rotate(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f), topFace, true);
}

void Cube::rotateBottomClockwise()
{
    rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f), bottomFace, true);
}

void Cube::rotateFrontAnticlockwise()
{
    rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f), frontFace, false);
}

void Cube::rotateBackAnticlockwise()
{
    rotate(-90.0f, glm::vec3(0.0f, 0.0f, 1.0f), backFace, false);
}

void Cube::rotateLeftAnticlockwise()
{
    rotate(-90.0f, glm::vec3(1.0f, 0.0f,0.0f), leftFace, false);
}

void Cube::rotateRightAnticlockwise()
{
    rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f), rightFace, false);
}

void Cube::rotateTopAnticlockwise()
{
    rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f), topFace, false);
}

void Cube::rotateBottomAnticlockwise()
{
    rotate(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f), bottomFace, false);
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
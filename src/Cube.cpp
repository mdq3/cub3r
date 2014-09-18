
#include <iostream>
#include <cmath>

#include "../include/Cube.hpp"

Cube::Cube()
{

}

Cube::~Cube()
{
    // Release resources
}

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

void Cube::rotateFront(GLfloat angle)
{
    std::vector<int> faces = {2, 3, 6, 7, 9, 10, 11, 12, 22};
    if(rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f), faces))
    {
        Model temp = cubes[7];
        cubes[7] = cubes[6];
        cubes[6] = cubes[2];
        cubes[2] = cubes[3];
        cubes[3] = temp;
        temp = cubes[12];
        cubes[12] = cubes[10];
        cubes[10] = cubes[9];
        cubes[9] = cubes[11];
        cubes[11] = temp;
    }
}

void Cube::rotateBack(GLfloat angle)
{
    std::vector<int> faces = {0, 1, 4, 5, 8, 13, 15, 17, 20};
    if(rotate(angle, glm::vec3(0.0f, 0.0f,1.0f), faces))
    {
        Model temp = cubes[5];
        cubes[5] = cubes[1];
        cubes[1] = cubes[0];
        cubes[0] = cubes[4];
        cubes[4] = temp;
        temp = cubes[17];
        cubes[17] = cubes[15];
        cubes[15] = cubes[8];
        cubes[8] = cubes[13];
        cubes[13] = temp;
    }
}

void Cube::rotateLeft(GLfloat angle)
{
    std::vector<int> faces = {0, 2, 4, 6, 10, 13, 14, 18, 23};
    if(rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f), faces))
    {
        Model temp = cubes[6];
        cubes[6] = cubes[4];
        cubes[4] = cubes[0];
        cubes[0] = cubes[2];
        cubes[2] = temp;
        temp = cubes[18];
        cubes[18] = cubes[13];
        cubes[13] = cubes[14];
        cubes[14] = cubes[10];
        cubes[10] = temp;
    }
}

void Cube::rotateRight(GLfloat angle)
{
    std::vector<int> faces = {1, 3, 5, 7, 11, 15, 16, 19, 24};
    if(rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f), faces))
    {
        Model temp = cubes[7];
        cubes[7] = cubes[3];
        cubes[3] = cubes[1];
        cubes[1] = cubes[5];
        cubes[5] = temp;
        temp = cubes[19];
        cubes[19] = cubes[11];
        cubes[11] = cubes[16];
        cubes[16] = cubes[15];
        cubes[15] = temp;
    }
}

void Cube::rotateTop(GLfloat angle)
{
    std::vector<int> faces = {4, 5, 6, 7, 12, 17, 18, 19, 25};
    if(rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f), faces))
    {
        Model temp = cubes[7];
        cubes[7] = cubes[5];
        cubes[5] = cubes[4];
        cubes[4] = cubes[6];
        cubes[6] = temp;
        temp = cubes[12];
        cubes[12] = cubes[19];
        cubes[19] = cubes[17];
        cubes[17] = cubes[18];
        cubes[18] = temp;
    }
}

void Cube::rotateBottom(GLfloat angle)
{
    std::vector<int> faces = {0, 1, 2, 3, 8, 9, 14, 16, 21};
    if(rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f), faces))
    {
        Model temp = cubes[3];
        cubes[3] = cubes[2];
        cubes[2] = cubes[0];
        cubes[0] = cubes[1];
        cubes[1] = temp;
        temp = cubes[9];
        cubes[9] = cubes[14];
        cubes[14] = cubes[8];
        cubes[8] = cubes[16];
        cubes[16] = temp;
    }
}

bool Cube::rotate(GLfloat angle, glm::vec3 axis, std::vector<int> faces)
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
        for(unsigned int i = 0; i < faces.size(); ++i)
        {
            cubes[faces[i]].localRotate(angle, axis, 0.05f);
        }
        return true;
    }
    return false;
}
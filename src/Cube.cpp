
#include <iostream>
#include <cmath>

#include "../include/Cube.hpp"

Cube::Cube() :
currentSlerpVal{0.00f}
{

}

Cube::~Cube()
{
    // Release resources
}

void Cube::setupCube(GLuint shaderProgram)
{
    Importer importer("resources/models/cub3.q3d");

    std::vector<Importer::Object> objects = importer.getObject();
    for(Importer::Object& object : objects)
    {
        Cub3 cube;
        std::vector<Importer::Mesh> meshes = object.meshes;
        for(Importer::Mesh& mesh : meshes)
        {
            std::vector<glm::vec3> vs = mesh.vs;
            std::vector<glm::vec3> ns = mesh.ns;
            std::vector<glm::vec2> uv = mesh.uvs;
            std::string texture       = mesh.texturePath;

            GLuint vertexCount = mesh.vsSize;

            Model model(vs, ns, uv, texture, vertexCount, shaderProgram, true);
            cube.models.push_back(model);
        }
        cubes.push_back(cube);
    }
}

void Cube::render(glm::mat4 viewProjectionMatrix)
{
    for(Cub3& cube : cubes)
    {
        for(Model& model : cube.models)
        {
            model.render(viewProjectionMatrix);
        }
    }
}

void Cube::setRotation(GLfloat angle, glm::vec3 axis, GLfloat rate)
{
    if(currentSlerpVal == 0.0f)
    {
        currentRotationAngle = angle;
        currentRotationAxis = axis;
        slerpRate = rate;
        currentSlerpVal += slerpRate;
    }
}

void Cube::rotate()
{
    for(Cub3& cube : cubes)
    {
        for(Model& model : cube.models)
        {
            model.worldRotate(currentRotationAngle, currentRotationAxis, currentSlerpVal);
        }
    }

    currentSlerpVal += slerpRate;
    currentSlerpVal = floor(currentSlerpVal * pow(10.0f, 2) + 0.5f) / pow(10.0f, 2);
    if(currentSlerpVal > 1.0f)
    {
        currentSlerpVal = 0.0f;
    }
}

void Cube::operations()
{
    if(currentSlerpVal != 0.0f)
    {
        rotate();
    }
}
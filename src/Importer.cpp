
#include <iostream>
#include <string>
#include <sstream>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "../include/Importer.hpp"

#define DOCTYPE "q3d" // The 3D geometry file format

Importer::Importer(std::string fileName)
{
    loadXML(fileName);
    loadModel();
}

Importer::~Importer() {}

std::vector<Importer::Mesh> Importer::getObjects()
{
    return objects;
}

void Importer::loadXML(std::string fileName)
{
    doc.LoadFile(fileName.c_str());
    if(doc.Error())
    {
        std::cout << "Error: could not load XML file " << fileName << '\n';
        exit(1);
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    const char* docType = root->Value();
    if(!strcmp(docType, DOCTYPE) == 0)
    {
        std::cout << "Error: file " << fileName << " is of wrong doc type\n";
        exit(1);
    }
}

void Importer::loadModel()
{
    tinyxml2::XMLNode* info = doc.RootElement()->FirstChild();
    tinyxml2::XMLNode* object = info->NextSibling();
    while(object != nullptr)
    {
        loadObject(object);
        object = object->NextSibling();
    }
}

void Importer::loadObject(tinyxml2::XMLNode* object)
{
    tinyxml2::XMLNode* mesh = object->FirstChild();
    while(mesh != nullptr)
    {
        tinyxml2::XMLNode* vertexPositions = mesh->FirstChild();
        tinyxml2::XMLNode* vertexNormals   = vertexPositions->NextSibling();
        tinyxml2::XMLNode* uvCoords        = vertexNormals->NextSibling();
        tinyxml2::XMLNode* texture         = uvCoords->NextSibling();

        int vsSize = atoi(vertexPositions->ToElement()->Attribute("count", nullptr));

        std::string vsString = std::string(vertexPositions->ToElement()->GetText());
        std::vector<glm::vec3> vertices = toVertexArray(vsSize, vsString);

        std::string nsString = std::string(vertexNormals->ToElement()->GetText());
        std::vector<glm::vec3> normals = toVertexArray(vsSize, nsString);

        std::string uvString = std::string(uvCoords->ToElement()->GetText());
        std::vector<glm::vec2> uvs = toUVArray(vsSize, uvString);

        std::string texturePath = std::string(texture->ToElement()->GetText());

        Mesh m = {vertices, normals, uvs, texturePath, vsSize};
        objects.push_back(m);

        mesh = mesh->NextSibling();
    }
}

std::vector<glm::vec3> Importer::toVertexArray(int count, std::string dataString)
{
    std::istringstream vertexStream(dataString);
    std::vector<glm::vec3> vertices;
    for(auto i = 0; i < count; ++i)
    {
        float v1;
        float v2;
        float v3;
        vertexStream >> v1;
        vertexStream >> v2;
        vertexStream >> v3;
        vertices.push_back(glm::vec3(v1, v2, v3));
    }
    return vertices;
}

std::vector<glm::vec2> Importer::toUVArray(int count, std::string dataString)
{
    std::istringstream uvStream(dataString);
    std::vector<glm::vec2> uvs;
    for(auto i = 0; i < count; ++i)
    {
        float v1;
        float v2;
        uvStream >> v1;
        uvStream >> v2;
        uvs.push_back(glm::vec2(v1, v2));
    }
    return uvs;
}
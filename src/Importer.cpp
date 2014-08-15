
#include <iostream>
#include <string>
#include <sstream>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "../include/Importer.hpp"


#define DOCTYPE "exporter"

Importer::Importer(std::string fileName)
{
    loadXML(fileName);
    loadModel();
}

Importer::~Importer()
{
    // Release resources
}

std::vector<Importer::Object> Importer::getObject()
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
    Object o;
    tinyxml2::XMLNode* mesh = object->FirstChild();
    while(mesh != nullptr)
    {
        tinyxml2::XMLNode* vertexPositions = mesh->FirstChild();
        tinyxml2::XMLNode* vertexNormals   = vertexPositions->NextSibling();
        tinyxml2::XMLNode* faceIndices     = vertexNormals->NextSibling();
        tinyxml2::XMLNode* color           = faceIndices->NextSibling();

        int vsSize = atoi(vertexPositions->ToElement()->Attribute("count", nullptr));
        int isSize = atoi(faceIndices->ToElement()->Attribute("count", nullptr));

        std::string vsString = std::string(vertexPositions->ToElement()->GetText());
        std::vector<glm::vec3> vertices = toVertexArray(vsSize, vsString);

        std::string nsString = std::string(vertexNormals->ToElement()->GetText());
        std::vector<glm::vec3> normals = toVertexArray(vsSize, nsString);

        std::string isString = std::string(faceIndices->ToElement()->GetText());
        std::vector<GLuint> indices = toIndexArray(isSize, isString);

        std::string colorString = std::string(color->ToElement()->GetText());
        std::vector<glm::vec3> colors = toColorArray(vsSize, colorString);

        Mesh m = {vertices, normals, indices, colors, vsSize, isSize};
        o.meshes.push_back(m);

        mesh = mesh->NextSibling();
    }
    objects.push_back(o);
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

std::vector<GLuint> Importer::toIndexArray(int count, std::string dataString)
{
    std::istringstream indexStream(dataString);
    std::vector<GLuint> indices;
    for(auto i = 0; i < count; ++i)
    {
        GLuint index;
        indexStream >> index;

        indices.push_back(index);
    }
    return indices;
}

std::vector<glm::vec3> Importer::toColorArray(int count, std::string dataString)
{
    std::istringstream colorStream(dataString);
    glm::vec3 color;
    std::vector<glm::vec3> colors;
    for(auto i = 0; i < 3; ++i)
    {
        float v1;
        float v2;
        float v3;

        colorStream >> v1;
        colorStream >> v2;
        colorStream >> v3;

        color = glm::vec3(v1, v2, v3);
    }
    for(auto i = 0; i < count; ++i)
    {
        colors.push_back(color);
    }
    return colors;
}
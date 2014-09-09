
#ifndef IMPORTER_H_
#define IMPORTER_H_


#include <vector>

#include <glm/glm.hpp>
#include <tinyxml2.h>


/**
 *
 */
class Importer {
 public:
    struct Mesh
    {
        std::vector<glm::vec3> vs;  // Vertices
        std::vector<glm::vec3> ns;  // Normals
        std::vector<glm::vec2> uvs; // UV coords
        std::string texturePath;    // Path to texture image
        int vsSize;                 // Number of vertices in mesh
    };

    struct Object
    {
        std::vector<Mesh> meshes;
    };

    Importer(std::string fileName);

    ~Importer();

    std::vector<Object> getObject();

 private:
    tinyxml2::XMLDocument doc;
    std::vector<Object> objects;

    void loadXML(std::string fileName);

    void loadModel();

    void loadObject(tinyxml2::XMLNode* object);

    std::vector<glm::vec3> toVertexArray(int count, std::string dataString);

    std::vector<glm::vec2> toUVArray(int count, std::string dataString);
};

#endif // IMPORTER_H_
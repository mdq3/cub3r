/**
 * Importer for q3d file format. Parses the XML and stores the data in a structure for
 * generation of 3D graphics.
 *
 * @author mdq3
 */

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

    Importer(std::string fileName);

    ~Importer();

    std::vector<Mesh> getObjects();

 private:
    tinyxml2::XMLDocument doc;
    std::vector<Mesh> objects;

    void loadXML(std::string fileName);

    void loadModel();

    void loadObject(tinyxml2::XMLNode* object);

    std::vector<glm::vec3> toVertexArray(int count, std::string dataString);

    std::vector<glm::vec2> toUVArray(int count, std::string dataString);
};

#endif // IMPORTER_H_
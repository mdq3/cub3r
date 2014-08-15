
#ifndef IMPORTER_H_
#define IMPORTER_H_


#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <tinyxml2.h>


/**
 *
 */
class Importer {
 public:
    struct Mesh
    {
        std::vector<glm::vec3> vs; // Vertices
        std::vector<glm::vec3> ns; // Normals
        std::vector<GLuint>    is; // Indices
        std::vector<glm::vec3> cs; // Colors

        int vsSize; // Number of vertices in mesh
        int isSize; // Number of face indices in mesh
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

    std::vector<GLuint> toIndexArray(int count, std::string dataString);

    std::vector<glm::vec3> toColorArray(int count, std::string dataString);
};

#endif // IMPORTER_H_
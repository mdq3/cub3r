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
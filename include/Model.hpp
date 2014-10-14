/**
 * 3D model. Holds geometry data and other attributes. Provides various transforms and sherical
 * linear interpolation for rotation.
 *
 * @author mdq3
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include <string>

#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 *
 */
class Model {
 public:
    Model();

    /**
     * Constructor for Model.
     */
    Model(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs,
          std::string texturePath, GLuint vCount, GLuint shader, GLfloat shininess, bool dynamicDraw);

    /**
     * Destructor for Model. Release resources.
     */
    ~Model();

    void scale(glm::vec3 amount);

    void translate(glm::vec3 amount);

    void localRotate(GLfloat angle, glm::vec3 axis, GLfloat slerp);

    void worldRotate(GLfloat angle, glm::vec3 axis, GLfloat slerp);

    void renderShadowMap(GLuint shadowProgram);

    void render(glm::mat4 viewProjectionMatrix, glm::mat4 depthBiasMVP);

    void operations();

    bool isRotating();

 private:
    GLuint VBOposition;      // Vertex Buffer Object for vertex positions
    GLuint VBOnormal;        // The vertex normals for this model
    GLuint VBOuv;            // The UV coordinates for this model's texture
    GLuint texture;          // This model's texture
    GLuint shaderProgram;    // This model's shader program
    GLuint vertexCount;      // The number of vertices in this model

    // Material Properties
    GLfloat materialShininess;

    glm::mat4 modelMatrix;            // The complete model transform matrix
    glm::mat4 modelScaleMatrix;       // The model's scale matrix
    glm::mat4 modelTranslateMatrix;   // The model's translation matrix
    glm::mat4 modelWorldRotateMatrix; // The model's world rotation matrix
    glm::mat4 modelLocalRotateMatrix; // The model's local rotation matrix

    glm::mat4 currentModelWorldRotateMatrix;

    GLfloat slerpRate;
    GLfloat currentSlerpVal; // Current slerp value. If > 0, rotation in progress, if >= 1, rotation finished
    GLfloat currentRotationAngle;
    glm::vec3 currentRotationAxis;

    void createVBO(GLuint& VBO, std::vector<glm::vec3> data, GLenum usage);

    void createUVBuffer(GLuint& VBOuv, std::vector<glm::vec2> data, GLenum usage);

    void createTexture(std::string filePath);

    void generateVertexNormals(std::vector<glm::vec3> vertices, std::vector<GLuint> indices, GLenum usage);

    void rotate(glm::mat4& rotateMatrix, GLfloat angle, glm::vec3 axis, GLfloat slerp);

    void doRotation();
};

#endif // MODEL_H_

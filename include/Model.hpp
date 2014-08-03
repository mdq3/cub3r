
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

 	/**
 	 * Constructor for Model.
 	 */
	Model(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<GLuint>& indices,
	      std::vector<glm::vec3> colors, GLuint iCount, GLuint shader, bool dynamicDraw);

	/**
	 * Destructor for Model. Release resources.
	 */
	~Model();

	void scale(glm::vec3 amount);

	void translate(glm::vec3 amount);

	void localRotate(GLfloat angle, glm::vec3 axis, GLfloat slerp);

	void worldRotate(GLfloat angle, glm::vec3 axis, GLfloat slerp);

	/**
	 * Get this model's shader program
	 *
	 * @return the GLuint for the shader program
	 */
	GLuint getShaderProgram();

	void render(glm::mat4 viewProjectionMatrix);

 private:
	GLuint VBOposition;      // Vertex Buffer Object for vertex positions
	GLuint VBOcolor;         // Vertex Buffer Object for vertex colors
	GLuint IBO;              // Index Buffer Object for this model
	GLuint VBOnormal;        // The vertex normals for this model

	GLuint indexCount;       // The number of vertex indices in this model

	// Material Properties
	GLfloat materialShininess;
	glm::vec3 materialSpecularColor;

	GLuint shaderProgram;             // This model's shader program

	glm::mat4 modelMatrix;            // The complete model transform matrix
	glm::mat4 modelScaleMatrix;       // The model's scale matrix
	glm::mat4 modelTranslateMatrix;   // The model's translation matrix
	glm::mat4 modelWorldRotateMatrix; // The model's world rotation matrix
	glm::mat4 modelLocalRotateMatrix; // The model's local rotation matrix

	glm::mat4 currentModelWorldRotateMatrix;


	void createVBO(GLuint& VBO, std::vector<glm::vec3> data, GLenum usage);

	void createIBO(std::vector<GLuint> data, GLenum usage);

	void generateVertexNormals(std::vector<glm::vec3> vertices, std::vector<GLuint> indices, GLenum usage);

	void rotate(glm::mat4& rotateMatrix, GLfloat angle, glm::vec3 axis, GLfloat slerp);
};

#endif // MODEL_H_

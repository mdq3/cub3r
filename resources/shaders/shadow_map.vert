#version 330

layout (location = 0) in vec3 vertexPosition;

uniform mat4 shadowViewProjectionMatrix;
uniform mat4 modelMatrix;

void main()
{
    gl_Position = (shadowViewProjectionMatrix * modelMatrix) * vec4(vertexPosition, 1.0);
}
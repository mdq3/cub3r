#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;

uniform mat4 modelViewProjectionMatrix;

out vec3 Color;

void main()
{
	Color = VertexColor;
	gl_Position = modelViewProjectionMatrix * vec4(VertexPosition, 1.0);
}

#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 modelViewProjectionMatrix;

out vec3 fragVert;
out vec3 fragColor;
out vec3 fragNormal;

void main()
{
	fragVert   = VertexPosition;
	fragColor  = VertexColor;
	fragNormal = VertexNormal;

	gl_Position = modelViewProjectionMatrix * vec4(VertexPosition, 1.0);
}

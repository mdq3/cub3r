#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;

uniform mat4 modelTransformMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 depthBiasMVP;

out vec3 fragVert;
out vec3 fragNormal;
out vec2 fragUV;
out vec4 shadowCoord;

void main()
{
    fragVert   = vertexPosition;
    fragNormal = vertexNormal;
    fragUV = vertexUV;

    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
    shadowCoord = depthBiasMVP * modelTransformMatrix * vec4(vertexPosition, 1.0);
}

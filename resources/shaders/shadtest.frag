#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D textureSampler;

in vec2 UV;

void main()
{
    color = texture2D(textureSampler, UV);
}
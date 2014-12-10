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

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

uniform struct Light
{
    vec3 position;
    vec3 rgb;
    float attenuation;
    vec3 direction;
    bool isPoint;
} light;

uniform float ambientLight;

uniform sampler2D textureSampler;
uniform sampler2DShadow shadowMap;

uniform float materialShininess;
uniform vec3  materialSpecularColor;

uniform mat4 modelTransformMatrix;
uniform mat3 normalMatrix;
uniform vec3 cameraPosition;

in vec3 fragVert;
in vec3 fragNormal;
in vec2 fragUV;
in vec4 shadowCoord;

out vec4 color;

void main()
{
    vec3 normal = normalize(normalMatrix * fragNormal);

    // Surface attributes
    vec3 surfacePosition = vec3(modelTransformMatrix * vec4(fragVert, 1.0));
    vec4 surfaceColor    = texture2D(textureSampler, fragUV);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePosition);
    vec3 surfaceToLight  = light.direction;
    if(light.isPoint)
    {
        surfaceToLight = normalize(light.position - surfacePosition);
    }

    // Ambient component
    vec3 ambient = ambientLight * surfaceColor.rgb * light.rgb;

    // Diffuse component
    float diffuse = max(0.0, dot(normal, surfaceToLight));

    // Specular component
    float specular = 0.0;
    if(diffuse > 0.0 && materialShininess > 0.0)
    {
        vec3 incidenceVector  = -surfaceToLight;
        vec3 reflectionVector = reflect(incidenceVector, normal);
        float cosAngle        = max(0.0, dot(surfaceToCamera, reflectionVector));
        specular              = pow(cosAngle, materialShininess);
    }

    // Light attenuation
    float distanceToLight = length(light.position - surfacePosition);
    float attenuation     = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2.0));

    // Shadow
    float sum = 0;
    sum += textureProjOffset(shadowMap, shadowCoord, ivec2(-1.0, -1.0));
    sum += textureProjOffset(shadowMap, shadowCoord, ivec2(-1.0, 1.0));
    sum += textureProjOffset(shadowMap, shadowCoord, ivec2(1.0, 1.0));
    sum += textureProjOffset(shadowMap, shadowCoord, ivec2(1.0, -1.0));
    float shadow = sum * 0.25;
    //shadow = textureProj(shadowMap, shadowCoord);

    if(shadow == 0.0)
    {
        specular = 0.0;
    }

    // Linear color
    vec3 scatteredLight = ambient + light.rgb * diffuse * shadow;
    vec3 reflectedLight = light.rgb * specular * attenuation;

    vec3 linearColor = min(surfaceColor.rgb * scatteredLight + reflectedLight, vec3(1.0));
    color = vec4(linearColor, surfaceColor.a);

    // Final color (with gamma correction)
    //vec3 gamma = vec3(1.0 / 2.2);
    //color      = vec4(pow(linearColor, gamma), 1.0);
}
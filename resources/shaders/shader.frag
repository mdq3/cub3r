#version 330

uniform struct Light
{
    vec3 position;
    vec3 rgb;
    float attenuation;
    float ambientCoefficient;
    vec3 direction;
    bool isPoint;
} light;

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
    vec3 surfacePosition = vec3(modelTransformMatrix * vec4(fragVert, 1));
    vec4 surfaceColor    = texture2D(textureSampler, fragUV);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePosition);
    vec3 surfaceToLight  = light.direction;
    if(light.isPoint)
    {
        surfaceToLight = normalize(light.position - surfacePosition);
    }

    // Ambient component
    vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.rgb;

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
    float attenuation     = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

    // Shadow
    float sum = 0;
    sum += textureProjOffset(shadowMap, shadowCoord, ivec2(-1,-1));
    sum += textureProjOffset(shadowMap, shadowCoord, ivec2(-1,1));
    sum += textureProjOffset(shadowMap, shadowCoord, ivec2(1,1));
    sum += textureProjOffset(shadowMap, shadowCoord, ivec2(1,-1));
    float shadow = sum * 0.25;
    //shadow = textureProj(shadowMap, shadowCoord);

    if(shadow == 0.0)
    {
        specular = 0.0;
    }

    // Linear color
    vec3 scatteredLight = ambient + light.rgb * diffuse * shadow;
    vec3 reflectedLight = light.rgb * specular * attenuation;

    vec3 linearColor    = min(surfaceColor.rgb * scatteredLight + reflectedLight, vec3(1.0));
    color = vec4(linearColor, surfaceColor.a);
    //color = textureProj(shadowMap, shadowCoord) * vec4(1.0);

    // Final color (with gamma correction)
    //vec3 gamma = vec3(1.0 / 2.2);
    //color      = vec4(pow(linearColor, gamma), 1.0);
}
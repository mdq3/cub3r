#version 330

uniform struct Light
{
	vec3 position;
	vec3 rgb;
	float attenuation;
	float ambientCoefficient;
} light;

uniform sampler2D sampler;

uniform float materialShininess;
uniform vec3  materialSpecularColor;

uniform mat4 modelTransformMatrix;
uniform vec3 cameraPosition;

in vec3 fragVert;
in vec3 fragNormal;
in vec2 fragUV;

out vec4 color;

void main()
{
	// Transform local vertex normal to world coordinates
	mat3 normalMatrix = transpose(inverse(mat3(modelTransformMatrix)));
	vec3 normal       = normalize(normalMatrix * fragNormal);

	// Surface attributes
	vec3 surfacePosition = vec3(modelTransformMatrix * vec4(fragVert, 1));
	vec4 surfaceColor    = texture2D(sampler, fragUV);
	vec3 surfaceToLight  = normalize(light.position - surfacePosition);
	vec3 surfaceToCamera = normalize(cameraPosition - surfacePosition);

	// Ambient lighting
	vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.rgb;

	// Diffuse lighting
	float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
	vec3 diffuse             = diffuseCoefficient * surfaceColor.rgb * light.rgb;

	// Specular lighting
	float specularCoefficient = 0.0;
	if(diffuseCoefficient > 0.0)
	{
		vec3 incidenceVector  = -surfaceToLight;
		vec3 reflectionVector = reflect(incidenceVector, normal);
		vec3 surfaceToCamera  = normalize(cameraPosition - surfacePosition);
		float cosAngle        = max(0.0, dot(surfaceToCamera, reflectionVector));
		specularCoefficient   = pow(cosAngle, materialShininess);
	}
	vec3 specular = specularCoefficient * materialSpecularColor * light.rgb;

	// Light attenuation
	float distanceToLight = length(light.position - surfacePosition);
	float attenuation     = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

	// Linear color
	vec3 linearColor = ambient + attenuation * (diffuse + specular);
	color = vec4(linearColor, 1.0);

	// Final color (with gamma correction)
	//vec3 gamma = vec3(1.0 / 2.2);
	//color      = vec4(pow(linearColor, gamma), 1.0);
}
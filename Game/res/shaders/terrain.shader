#shader vertex
#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normal;

out vec2 pass_texCoords;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;
out float visibility;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 lightPosition;

const float density = 0.0035;
const float gradient = 5.0;

void main()
{
	vec4 worldPosition = model * vec4(position, 1.0);

	vec4 positionRelativeToCam = view * worldPosition;
	gl_Position = projection * positionRelativeToCam;
	pass_texCoords = texCoords;
	surfaceNormal = (model * vec4(normal, 0.0)).xyz;
	toLightVector = lightPosition - worldPosition.xyz;
	toCameraVector = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance * density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);
};

#shader fragment
#version 330 core

in vec2 pass_texCoords;
in vec3 surfaceNormal;
in vec3 toLightVector; 
in vec3 toCameraVector;
in float visibility;

out vec4 out_Colour;

uniform sampler2D texSampler;
uniform vec3 lightColour;
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColour;

void main()
{
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);

	float nDot1 = dot(unitNormal, unitLightVector);
	float brightness = max(nDot1, 0.2);
	vec3 diffuse = brightness * lightColour;

	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

	float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
	specularFactor = max(specularFactor, 0.0);
	float dampedFactor = pow(specularFactor, shineDamper);
	vec3 finalSpecular = dampedFactor * lightColour * reflectivity;

	out_Colour = vec4(diffuse, 1.0) * texture(texSampler, pass_texCoords) + vec4(finalSpecular, 1.0);
	out_Colour = mix(vec4(skyColour, 1.0), out_Colour, visibility);
};
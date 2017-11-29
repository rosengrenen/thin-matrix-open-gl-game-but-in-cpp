#shader vertex
#version 400 core

in vec3 position;
in vec2 textureCoordinates;
in vec3 normal;
in vec3 tangent;

out vec2 pass_textureCoordinates;
out vec3 toLightVector[4];
out vec3 toCameraVector;
out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition[4];

uniform float numberOfRows;
uniform vec2 offset;

const float density = 0;
const float gradient = 5.0;

uniform vec4 plane;

void main(void)
{
	vec4 worldPosition = model * vec4(position, 1.0);

	gl_ClipDistance[0] = dot(worldPosition, plane);

	mat4 modelViewMatrix = view * model;

	vec4 positionRelativeToCam = modelViewMatrix * vec4(position, 1.0);

	gl_Position = projection * positionRelativeToCam;

	pass_textureCoordinates = (textureCoordinates / numberOfRows) + offset;

	vec3 surfaceNormal = (modelViewMatrix * vec4(normal, 0.0)).xyz;

	vec3 norm = normalize(surfaceNormal);
	vec3 tang = normalize((view * model * vec4(tangent, 1.0)).xyz);
	vec3 bitang = normalize(cross(norm, tang));

	mat3 toTangentSpace = mat3(
		tang.x, bitang.x, norm.x,
		tang.y, bitang.y, norm.y,
		tang.z, bitang.z, norm.z
	);

	for (int i = 0; i < 4; i++)
	{
		toLightVector[i] = toTangentSpace * ((view * vec4(lightPosition[i], 1.0)).xyz - worldPosition.xyz);
	}
	toCameraVector = toTangentSpace * (-positionRelativeToCam.xyz);

	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance * density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);
}

#shader fragment
#version 400 core

in vec2 pass_textureCoordinates;
in vec3 toLightVector[4];
in vec3 toCameraVector;
in float visibility;

out vec4 out_Colour;

uniform sampler2D modelTexture;
uniform sampler2D normalMap;
uniform vec3 lightColour[4];
uniform vec3 attenuation[4];
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColour;

void main(void)
{

	vec4 normalMapValue = 2.0 * texture(normalMap, pass_textureCoordinates) - 1.0;

	vec3 unitNormal = normalize(normalMapValue.rgb);
	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 totalDiffuse = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);

	for (int i = 0; i < 4; i++)
	{
		float distance = length(toLightVector[i]);
		float attenuationFactor = attenuation[i].x + attenuation[i].y * distance + attenuation[i].z * distance * distance;
		vec3 unitLightVector = normalize(toLightVector[i]);
		float nDot1 = dot(unitNormal, unitLightVector);
		float brightness = max(nDot1, 0.0);
		/* CEL SHADING
		brightness = floor(brightness * levels) / levels;*/
		vec3 lightDirection = -unitLightVector;
		vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
		float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
		specularFactor = max(specularFactor, 0.0);
		float dampedFactor = pow(specularFactor, shineDamper);
		/* CEL SHADING
		dampedFactor = floor(dampedFactor * levels) / levels; */
		totalDiffuse = totalDiffuse + brightness * lightColour[i] / attenuationFactor;
		totalSpecular = totalSpecular + dampedFactor * lightColour[i] * reflectivity / attenuationFactor;
	}
	totalDiffuse = max(totalDiffuse, 0.2);

	vec4 textureColour = texture(modelTexture, pass_textureCoordinates);
	if (textureColour.a < 0.5)
	{
		discard;
	}

	out_Colour = vec4(totalDiffuse, 1.0) * textureColour + vec4(totalSpecular, 1.0);
	out_Colour = mix(vec4(skyColour, 1.0), out_Colour, visibility);
}
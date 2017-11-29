#shader vertex
#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normal;

out vec2 pass_texCoords;

out vec3 surfaceNormal;
out vec3 toLightVector[4];
out vec3 toCameraVector;
out float visibility;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 lightPosition[4];

const float density = 0.0035;
const float gradient = 5.0;

uniform vec4 plane;

void main()
{
	vec4 worldPosition = model * vec4(position, 1.0);

	gl_ClipDistance[0] = dot(worldPosition, plane);

	vec4 positionRelativeToCam = view * worldPosition;

	gl_Position = projection * positionRelativeToCam;

	pass_texCoords = texCoords;

	surfaceNormal = (model * vec4(normal, 0.0)).xyz;
	for (int i = 0; i < 4; i++)
	{
		toLightVector[i] = lightPosition[i] - worldPosition.xyz;
	}
	toCameraVector = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance * density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);
};

#shader fragment
#version 330 core

in vec2 pass_texCoords;
in vec3 surfaceNormal;
in vec3 toLightVector[4];
in vec3 toCameraVector;
in float visibility;

out vec4 out_Colour;

uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;

uniform vec3 lightColour[4];
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColour;
uniform vec3 attenuation[4];

const float levels = 5.0;

void main()
{
	/* BLEND MAP TEXTURING */
	vec4 blendMapColour = texture(blendMap, pass_texCoords);

	float bgTextureAmount = 1 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);
	vec2 tiledCoords = pass_texCoords * 40.0;
	vec4 bgTexColour = texture(backgroundTexture, tiledCoords) * bgTextureAmount;
	vec4 rTexColour = texture(rTexture, tiledCoords) * blendMapColour.r;
	vec4 gTexColour = texture(gTexture, tiledCoords) * blendMapColour.g;
	vec4 bTexColour = texture(bTexture, tiledCoords) * blendMapColour.b;

	vec4 totalColour = bgTexColour + rTexColour + gTexColour + bTexColour;
	/* BLEND MAP TEXTURING STOP */


	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 totalDiffuse = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);

	for (int i = 0; i < 4; i++)
	{
		float distance = length(toLightVector[i]);
		float attenuationFactor = attenuation[i].x + attenuation[i].y * distance + attenuation[i].z * distance * distance;
		vec3 unitLightVector = normalize(toLightVector[i]);
		float nDot1 = dot(unitNormal, unitLightVector);
		float brightness = max(nDot1, 0.2);
		/* CEL SHADIN
		brightness = floor(brightness * levels) / levels;*/
		vec3 lightDirection = -unitLightVector;
		vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
		float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
		specularFactor = max(specularFactor, 0.0);
		float dampedFactor = pow(specularFactor, shineDamper);
		/* CEL SHADING
		dampedFactor = floor(dampedFactor * levels) / levels; */
		totalDiffuse += brightness * lightColour[i] / attenuationFactor;
		totalSpecular += dampedFactor * lightColour[i] * reflectivity / attenuationFactor;
	}

	totalDiffuse = max(totalDiffuse, 0.2);

	out_Colour = vec4(totalDiffuse, 1.0) * totalColour + vec4(totalSpecular, 1.0);
	out_Colour = mix(vec4(skyColour, 1.0), out_Colour, visibility);
};
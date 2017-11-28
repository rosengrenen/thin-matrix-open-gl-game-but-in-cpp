#shader vertex
#version 330 core

in vec3 position;
out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main(void)
{

	gl_Position = projection * view * vec4(position, 1.0);
	texCoords = position;

}

#shader fragment
#version 330 core

in vec3 texCoords;
out vec4 out_Colour;

uniform samplerCube cubeMap;
uniform samplerCube cubeMap2;
uniform vec3 fogColour;
uniform float blendFactor;

const float lowerLimit = -15.0;
const float upperLimit = 15.0;

const float levels = 10.0;

void main(void)
{
	vec4 texture1 = texture(cubeMap, texCoords);
	vec4 texture2 = texture(cubeMap2, texCoords);
	vec4 finalColour = mix(texture1, texture2, blendFactor);

	/* CEL SHADING
	float amount = (finalColour.r + finalColour.g + finalColour.b) / 3.0;
	amount = floor(amount * levels) / levels;
	finalColour.rgb = amount * fogColour;*/

	float factor = (texCoords.y - lowerLimit) / (upperLimit - lowerLimit);
	factor = clamp(factor, 0.0, 1.0);
	out_Colour = mix(vec4(fogColour, 1.0), finalColour, factor);
}
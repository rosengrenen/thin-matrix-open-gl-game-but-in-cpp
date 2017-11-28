#shader vertex
#version 400 core

in vec2 position;

out vec4 clipSpace;
out vec2 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

const float tiling = 6.0;

void main(void)
{
	clipSpace = projectionMatrix * viewMatrix * modelMatrix * vec4(position.x, 0.0, position.y, 1.0);
	gl_Position = clipSpace;
	textureCoords = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5) * tiling;
}

#shader fragment
#version 400 core

out vec4 out_Colour;

in vec4 clipSpace;
in vec2 textureCoords;

uniform sampler2D reflection;
uniform sampler2D refraction;
uniform sampler2D dudvMap;

uniform float moveFactor;

const float waveStrength = 0.02;

void main(void)
{
	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

	vec2 distortion1 = (texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 2.0 - 1.0) * waveStrength;
	vec2 distortion2 = (texture(dudvMap, vec2(-textureCoords.x + moveFactor, textureCoords.y + moveFactor)).rg * 2.0 - 1.0) * waveStrength;
	vec2 totalDistortion = distortion1 + distortion2;
	refractTexCoords += totalDistortion;
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

	reflectTexCoords += totalDistortion;
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

	vec4 reflectionColour = texture(reflection, reflectTexCoords);
	vec4 refractionColour = texture(refraction, refractTexCoords);

	out_Colour = mix(reflectionColour, refractionColour, 0.5);
	out_Colour = mix(out_Colour, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
}
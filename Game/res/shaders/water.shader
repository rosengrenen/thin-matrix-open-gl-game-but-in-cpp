#shader vertex
#version 400 core

in vec2 position;

out vec4 clipSpace;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;


void main(void)
{
	clipSpace = projectionMatrix * viewMatrix * modelMatrix * vec4(position.x, 0.0, position.y, 1.0);
	gl_Position = clipSpace;
}

#shader fragment
#version 400 core

out vec4 out_Color;

in vec4 clipSpace;

uniform sampler2D reflection;
uniform sampler2D refraction;

void main(void)
{
	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
	vec4 reflectionColour = texture(reflection, reflectTexCoords);
	vec4 refractionColour = texture(refraction, refractTexCoords);

	out_Color = mix(reflectionColour, refractionColour, 0.5);

}
#shader vertex
#version 330

in vec2 position;
in vec2 texCoord;

out vec2 pass_texCoord;

uniform vec2 translation;

void main(void)
{
	gl_Position = vec4(position, 1.0);
	pass_texCoord = texCoord;
}

#shader fragment
#version 330

in vec2 pass_texCoord

out vec4 out_Colour;

uniform vec3 colour;
uniform sampler2D fontAtlas;

void main(void)
{
	out_Colour = vec4(colour, texture(fontAtlas, pass_texCoords).a);
}
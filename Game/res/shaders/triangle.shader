#shader vertex
#version 330 core

in vec3 position;
in vec2 texCoords;

out vec2 pass_texCoords;

void main()
{
	gl_Position = vec4(position, 1.0);
	pass_texCoords = texCoords;
};

#shader fragment
#version 330 core

in vec2 pass_texCoords;

out vec4 out_Color;

uniform sampler2D texSampler;

void main()
{
	out_Color = texture(texSampler, pass_texCoords);
};
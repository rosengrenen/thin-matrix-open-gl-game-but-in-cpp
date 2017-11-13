#shader vertex
#version 330 core

in vec3 position;

out vec3 colour;

void main()
{
	gl_Position = vec4(position, 1.0);
	colour = position;
};

#shader fragment
#version 330 core

in vec3 colour;

out vec4 out_Color;

void main()
{
	vec3 otherVar = 0.5*(colour + 1.0);
	out_Color = vec4(otherVar, 1.0);
};
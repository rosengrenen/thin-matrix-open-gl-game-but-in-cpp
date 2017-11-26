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
out vec4 out_Color;

uniform samplerCube cubeMap;

void main(void)
{
	out_Color = texture(cubeMap, texCoords);
}
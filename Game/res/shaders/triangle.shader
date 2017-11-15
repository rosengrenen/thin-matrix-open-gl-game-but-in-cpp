#shader vertex
#version 330 core

in vec3 position;
in vec2 texCoords;

out vec2 pass_texCoords;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(position, 1.0);
	pass_texCoords = texCoords;
};

#shader fragment
#version 330 core

in vec2 pass_texCoords;

out vec4 out_Color;

uniform sampler2D texSampler;

void main()
{
	//out_Color = vec4(1.0, 0.1, 0.7, 1.0);
	out_Color = vec4(1.0, 0.1, 0.7, 1.0) * texture(texSampler, pass_texCoords);
};
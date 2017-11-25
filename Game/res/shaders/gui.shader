#shader vertex
#version 330

in vec2 position;

out vec2 texCoords;

uniform mat4 transformation;

void main(void)
{
	gl_Position = transformation * vec4(position, 0.0, 1.0);
	texCoords = vec2((position.x + 1.0) / 2.0, 1 - (position.y + 1.0) / 2.0);
}

#shader fragment
#version 140

in vec2 texCoords;

out vec4 out_Color;

uniform sampler2D guiTexture;

void main(void)
{
	out_Color = texture(guiTexture, texCoords);
}
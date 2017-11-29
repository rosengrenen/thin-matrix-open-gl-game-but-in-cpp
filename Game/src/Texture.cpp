#include "Texture.h"

Texture::Texture(const GLuint id) : m_id(id), numberOfRows(1)
{ }

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getID() const
{
	return m_id;
}
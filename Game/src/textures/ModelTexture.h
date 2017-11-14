#pragma once

class ModelTexture
{
private:
	int m_texture;
public:
	ModelTexture(const int id) : m_texture(id)
	{ }

	int getID() const
	{
		return m_texture;
	}
};
#define STB_IMAGE_IMPLEMENTATION
#include "TextureData.h"


TextureData::TextureData(const std::string& path, int forcedChannels)
{
	m_data = stbi_load(("res/textures/" + path).c_str(), &m_width, &m_height, &m_channels, forcedChannels);
	if (!m_data)
	{
		std::cout << "Error! Could not load texture: " << path.c_str() << std::endl;
		throw "Could not retrieve data from file: " + path;
	}
}

const int TextureData::getRGB(int x, int y) const
{
	return ((getR(x, y) << 8)
		+ getG(x, y) << 8)
		+ getB(x, y);
}

const unsigned char TextureData::getR(int x, int y) const
{
	int index = y * m_width * 4 + x * 4;
	if (index < m_width * m_height * 4)
		return m_data[index];
	return 0;
}

const unsigned char TextureData::getG(int x, int y) const
{
	int index = y * m_width * 4 + x * 4 + 1;
	if (index < m_width * m_height * 4)
		return m_data[index];
	return 0;
}

const unsigned char TextureData::getB(int x, int y) const
{
	int index = y * m_width * 4 + x * 4 + 2;
	if (index < m_width * m_height * 4)
		return m_data[index];
	return 0;
}

unsigned char TextureData::getA(int x, int y) const
{
	int index = y * m_width * 4 + x * 4 + 3;
	if (index < m_width * m_height * 4)
		return m_data[index];
	return 0;
}

const int TextureData::getWidth() const
{
	return m_width;
}

const int TextureData::getHeight() const
{
	return m_height;
}

const int TextureData::getChannels() const
{
	return m_channels;
}

const unsigned char* TextureData::getData() const
{
	return m_data;
}
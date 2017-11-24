#pragma once

#include <vector>

class Image
{
public:
	unsigned int id;
	int width;
	int height;
	int channels;
	std::vector<unsigned char> data;
public:
	Image(int id, int width, int height, int channels, const std::vector<unsigned char>& data) :
		id(id),
		width(width),
		height(height),
		channels(channels),
		data(data)
	{ }

	int getRGB(int x, int y) const
	{
		int rgb = getR(x, y);
		rgb = (rgb << 8) + getG(x, y);
		return (rgb << 8) + getB(x, y);
	}

	int getR(int x, int y) const
	{
		return data[y * width * 4 + x * 4];
	}

	int getG(int x, int y) const
	{
		return data[y * width * 4 + x * 4 + 1];
	}

	int getB(int x, int y) const
	{
		return data[y * width * 4 + x * 4 + 2];
	}

	int getA(int x, int y) const
	{
		return data[y * width * 4 + x * 4 + 3];
	}
};
#pragma once

#include <vector>

class Image
{
public:
	unsigned int id;
	int width;
	int height;
	int channels;
	std::vector<std::vector<unsigned char>> data;
public:
	Image(int id, int width, int height, int channels, const std::vector<std::vector<unsigned char>>& data) :
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
		return data.at(y).at(4 * x);
	}

	int getG(int x, int y) const
	{
		return data.at(y).at(4 * x + 1);
	}

	int getB(int x, int y) const
	{
		return data.at(y).at(4 * x + 2);
	}

	int getA(int x, int y) const
	{
		return data.at(y).at(4 * x + 3);
	}
};
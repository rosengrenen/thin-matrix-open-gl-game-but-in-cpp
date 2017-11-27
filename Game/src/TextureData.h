#pragma once

#include <iostream>
#include <vector>

#include "dependencies\stb_image.h"

class TextureData
{
private:
	int m_width;
	int m_height;
	int m_channels;
	unsigned char* m_data;
public:
	TextureData(const std::string& path, int forcedChannels = 4);

	// Returns the rgb value of a pixel;
	const int getRGB(int x, int y) const;

	// Returns the red channel value of a pixel, returns 0 if invalid range
	const unsigned char getR(int x, int y) const;

	// Returns the green channel value of a pixel, returns 0 if invalid range
	const unsigned char getG(int x, int y) const;

	// Returns the blue channel value of a pixel, returns 0 if invalid range
	const unsigned char getB(int x, int y) const;

	// Returns the alpha channel value of a pixel, returns 0 if invalid range
	unsigned char getA(int x, int y) const;

	// Returns the width of the texture in pixels
	const int getWidth() const;

	// Returns the height of the texture in pixels
	const int getHeight() const;

	// Returns the number of channels of the texture
	const int getChannels() const;

	// Returns the texture data
	const unsigned char* getData() const;
};
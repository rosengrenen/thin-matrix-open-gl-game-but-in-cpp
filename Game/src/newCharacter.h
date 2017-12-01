#pragma once

#include <unordered_map>

class Character
{
public:
	// The ASCII code
	int id;
	// The 
	int texOffsetX;
	int texOffsetY;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int xadvance;
	// Kerning <ascii, amount>
	std::unordered_map<int, int> kerning;
};
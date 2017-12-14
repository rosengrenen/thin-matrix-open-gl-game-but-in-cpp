#pragma once

#include <unordered_map>

class Character
{
public:
	// The ASCII code
	int id;
	// The 
	float texOffsetX;
	float texOffsetY;
	float width;
	float height;
	float xoffset;
	float yoffset;
	float xadvance;
	// Kerning <ascii, amount>
	std::unordered_map<int, int> kerning;

};
#pragma once

#include <string>
#include <unordered_map>

#include <fstream>

#include "Texture.h"

#include "newCharacter.h"

namespace
{
	struct Padding
	{
		int top;
		int right;
		int bottom;
		int left;
	};

	struct Spacing
	{
		int horizontal;
		int vertical;
	};
}

class Font
{
private:
	// Character will be some simple structure, which also contains the positional data
	std::unordered_map<int, newCharacter> m_characters;
	// The distance from one line to another
	int m_lineHeight;
	int m_base;
	int m_size;
	std::string m_name;
	bool m_bold;
	Padding m_padding;
	Spacing m_spacing;
	// The font image
	Texture m_texture;
public:
	Font(const std::string& path, const Texture& texture) : m_texture(texture)
	{
		std::ifstream stream("res/fonts/" + path + ".fnt");
		// Load info header
		// Load page header
	}

	newCharacter getCharacter(int asciiCode)
	{
		//TODO: check if the character exists
		return m_characters.at(asciiCode);
	}
};
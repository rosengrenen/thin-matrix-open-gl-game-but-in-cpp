#pragma once

#include <string>
#include <unordered_map>

#include <fstream>
#include <iostream>

#include "Texture.h"
#include "Utilities.h"

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
	const char SPLITTER = ' ';
	const char NUMBER_SEPARATOR = ',';
	// Character will be some simple structure, which also contains the positional data
	std::unordered_map<int, newCharacter> m_characters;
	// The distance from one line to another
	int m_lineHeight;
	int m_base;
	std::string m_name;
	//bool m_bold;
	//bool m_italic;
	Padding m_padding;
	Spacing m_spacing;
	// The font image
	Texture m_texture;
public:
	Font(const std::string& path, const Texture& texture) : m_texture(texture)
	{
		std::ifstream stream("res/fonts/" + path + ".fnt");
		std::string line;
		getInfoHeader(stream);
		getPageHeader(stream);
		// Load info header
		// Load page header
	}

	void getInfoHeader(std::ifstream& stream)
	{
		std::string line;
		getline(stream, line);
		std::vector<std::string> res1 { Utilities::stringExplode(line, SPLITTER) };
		for (std::string& s : res1)
		{
			std::vector<std::string> res2 { Utilities::stringExplode(s, '=') };
			if (res2[0].c_str() == std::string("face"))
			{
				m_name = res2[1].substr(1, res2[1].size() - 2);
			}
			else if (res2[0].c_str() == "size")
			{
			}
			else if (res2[0].c_str() == "bold")
			{
			}
			else if (res2[0].c_str() == "italic")
			{
			}
			else if (res2[0].c_str() == "charset")
			{
			}
			else if (res2[0].c_str() == "unicode")
			{
			}
			else if (res2[0].c_str() == "stretchH")
			{
			}
			else if (res2[0].c_str() == "smooth")
			{
			}
			else if (res2[0].c_str() == "aa")
			{
			}
			else if (res2[0].c_str() == std::string("padding"))
			{
				std::vector<std::string> padding { Utilities::stringExplode(res2[1], NUMBER_SEPARATOR) };
				if (padding.size() < 4)
					throw "Invalid padding values";
				m_padding = { ::atoi(padding[0].c_str()),
					::atoi(padding[1].c_str()),
					::atoi(padding[2].c_str()),
					::atoi(padding[3].c_str()) };
			}
			else if (res2[0].c_str() == std::string("spacing"))
			{
				std::vector<std::string> spacing { Utilities::stringExplode(res2[1], NUMBER_SEPARATOR) };
				if (spacing.size() < 2)
					throw "Invalid spacing values";
				m_spacing = { ::atoi(spacing[0].c_str()), ::atoi(spacing[1].c_str()) };
			}
		}
	}

	void getPageHeader(std::ifstream& stream)
	{
		std::string line;
	}

	newCharacter getCharacter(int asciiCode)
	{
		//TODO: check if the character exists
		return m_characters.at(asciiCode);
	}
};
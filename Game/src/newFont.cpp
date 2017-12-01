#include "newFont.h"

Font::Font(const std::string& path)
{
	std::ifstream stream("res/fonts/" + path + ".fnt");
	std::string line;
	while (getline(stream, line))
	{
		if (line.substr(0, 5) == "info ")
		{
			loadInfo(line);
		}
		else if (line.substr(0, 7) == "common ")
		{
			loadCommon(line);
		}
		else if (line.substr(0, 5) == "page ")
		{
			loadPage(line);
		}
		else if (line.substr(0, 5) == "char ")
		{
			loadCharacter(line);
		}
		else if (line.substr(0, 8) == "kerning ")
		{
			loadKerning(line);
		}
	}
}

Text Font::loadText(const std::string & text, float fontSize, const glm::vec2 & position, float maxLineLength, bool centered)
{
	return Text();
}


void Font::loadInfo(const std::string& line)
{
	std::vector<std::string> lineValues { Utilities::stringExplode(line, SPLITTER) };
	for (std::string& lineValue : lineValues)
	{
		std::vector<std::string> lineValueKeyValuePair { Utilities::stringExplode(lineValue, '=') };
		if (lineValueKeyValuePair[0] == "face")
		{
			m_name = lineValueKeyValuePair[1].substr(1, lineValueKeyValuePair[1].size() - 2);
		}
		else if (lineValueKeyValuePair[0] == "size")
		{
			m_size = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "bold")
		{
			m_bold = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "italic")
		{
			m_italic = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "charset")
		{
			m_charset = lineValueKeyValuePair[1].substr(1, lineValueKeyValuePair[1].size() - 2);
		}
		else if (lineValueKeyValuePair[0] == "unicode")
		{
			m_unicode = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "stretchH")
		{
			m_stretchH = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "smooth")
		{
			m_smooth = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "aa")
		{
			m_aa = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "padding")
		{
			std::vector<std::string> padding { Utilities::stringExplode(lineValueKeyValuePair[1], NUMBER_SEPARATOR) };
			if (padding.size() < 4)
				throw "Invalid padding values";
			m_padding = { ::atoi(padding[0].c_str()),
				::atoi(padding[1].c_str()),
				::atoi(padding[2].c_str()),
				::atoi(padding[3].c_str()) };
		}
		else if (lineValueKeyValuePair[0] == "spacing")
		{
			std::vector<std::string> spacing { Utilities::stringExplode(lineValueKeyValuePair[1], NUMBER_SEPARATOR) };
			if (spacing.size() < 2)
				throw "Invalid spacing values";
			m_spacing = { ::atoi(spacing[0].c_str()), ::atoi(spacing[1].c_str()) };
		}
		else if (lineValueKeyValuePair[0] == "outline")
		{
			m_outline = ::atoi(lineValueKeyValuePair[1].c_str());
		}
	}
}

void Font::loadCommon(const std::string& line)
{
	std::vector<std::string> lineValues { Utilities::stringExplode(line, SPLITTER) };
	for (std::string& lineValue : lineValues)
	{
		std::vector<std::string> lineValueKeyValuePair { Utilities::stringExplode(lineValue, '=') };
		if (lineValueKeyValuePair[0] == "lineHeight")
		{
			m_lineHeight = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "base")
		{
			m_base = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "scaleW")
		{
			m_scaleW = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "scaleH")
		{
			m_scaleH = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "pages")
		{

		}
		else if (lineValueKeyValuePair[0] == "packed")
		{

		}
	}
}

void Font::loadPage(const std::string& line)
{
	std::vector<std::string> lineValues { Utilities::stringExplode(line, SPLITTER) };
	for (std::string& lineValue : lineValues)
	{
		std::vector<std::string> lineValueKeyValuePair { Utilities::stringExplode(lineValue, '=') };
		if (lineValueKeyValuePair[0] == "file")
		{
			std::string file = lineValueKeyValuePair[1].substr(1, lineValueKeyValuePair[1].size() - 2);
			m_texture = std::make_unique<Texture>(Loader::loadTexture2D("../fonts/" + file));
		}
	}
}

void Font::loadCharacter(const std::string& line)
{
	std::vector<std::string> lineValues { Utilities::stringExplode(line, SPLITTER) };
	int id, x, y, width, height, xoffset, yoffset, xadvance;
	for (std::string& lineValue : lineValues)
	{
		if (lineValue == "")
			continue;
		std::vector<std::string> lineValueKeyValuePair { Utilities::stringExplode(lineValue, '=') };
		if (lineValueKeyValuePair[0] == "id")
		{
			id = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "x")
		{
			x = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "y")
		{
			y = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "width")
		{
			width = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "height")
		{
			height = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "xoffset")
		{
			xoffset = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "yoffset")
		{
			yoffset = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "xadvance")
		{
			xadvance = ::atoi(lineValueKeyValuePair[1].c_str());
		}
	}
	m_characters.insert({ id, { id, x, y, width, height, xoffset, yoffset, xadvance } });
}

void Font::loadKerning(const std::string& line)
{
	std::vector<std::string> lineValues { Utilities::stringExplode(line, SPLITTER) };
	int first, second, amount;
	for (std::string& lineValue : lineValues)
	{
		std::vector<std::string> lineValueKeyValuePair { Utilities::stringExplode(lineValue, '=') };
		if (lineValueKeyValuePair[0] == "first")
		{
			first = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "second")
		{
			second = ::atoi(lineValueKeyValuePair[1].c_str());
		}
		else if (lineValueKeyValuePair[0] == "amount")
		{
			amount = ::atoi(lineValueKeyValuePair[1].c_str());
		}
	}
	if (m_characters.find(first) != m_characters.end())
	{
		if (m_characters.at(first).kerning.find(second) == m_characters.at(first).kerning.end())
		{
			m_characters.at(first).kerning.insert({ second, amount });
		}
	}
}
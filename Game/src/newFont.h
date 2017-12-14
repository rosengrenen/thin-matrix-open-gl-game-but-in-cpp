#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <fstream>
#include <iostream>

#include "Loader.h"
#include "Texture.h"
#include "Utilities.h"

#include "newCharacter.h"
#include "newText.h"
#include "newWord.h"
#include "newLine.h"

class Font
{
private:
	/* CONSTANTS */
	const char SPLITTER = ' ';
	const char NUMBER_SEPARATOR = ',';
	/* INFO HEADER */
	std::string m_name; // face: the name of the true type font
	int m_size; // size: the size of the true type font
	bool m_bold; // bold: if the font is bold
	bool m_italic; // italic: if the font is italic
	std::string m_charset; // charset: the name of the EOM charset (when not unicode)
	bool m_unicode; // unicode: if using the unicode charset
	int m_stretchH; // stretchH: the font height stretch (100% means no stretch)
	bool m_smooth; // smooth: if smoothing is turned on
	int m_aa = 1; // aa: the supersampling level used (1 means no supersampling)
	struct Padding
	{
		int top;
		int right;
		int bottom;
		int left;
	} m_padding; // padding: the padding for each character (up, right, down, left)
	struct Spacing
	{
		int horizontal;
		int vertical;
	} m_spacing; // spacing: the spacing for each character (horizontal, vertical)
	int m_outline; // outline: the outline thickness for characters
	/* COMMON HEADER */
	int m_lineHeight; // lineHeight: the distance in pixels between each line of text
	int m_base; // base: the number of pixels from the absoluate top of the line to the base of characters
	int m_scaleW; // the width of the texture
	int m_scaleH; // the height of the texture
	/* FILE HEADER */
	std::unique_ptr<Texture> m_texture;
	/* END HEADERS */
	std::unordered_map<int, Character> m_characters;
public:
	Font(const std::string& path);

	Text loadText(const std::string& text, float fontSize, const glm::vec2& position, float maxLineLength, bool centered);
private:
	void loadInfo(const std::string& line);

	void loadCommon(const std::string& line);

	void loadPage(const std::string& line);

	void loadCharacter(const std::string& line);

	void loadKerning(const std::string& line);
};
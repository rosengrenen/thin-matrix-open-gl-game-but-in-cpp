#pragma once

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>

#include "FontType.h"

#include <string>

class GUIText
{
private:
	std::string m_textString;
	float m_fontSize;

	int m_textMeshVao;
	int m_vertexCount;
	glm::vec3 m_colour { 0.0f, 0.0f, 0.0f };

	glm::vec2 m_position;
	float m_lineMaxSize;
	int m_numberOfLines;

	FontType m_font;

	bool m_centerText = false;

	/**
	* Creates a new text, loads the text's quads into a VAO, and adds the text
	* to the screen.
	*
	* @param text
	*            - the text.
	* @param fontSize
	*            - the font size of the text, where a font size of 1 is the
	*            default size.
	* @param font
	*            - the font that this text should use.
	* @param position
	*            - the position on the screen where the top left corner of the
	*            text should be rendered. The top left corner of the screen is
	*            (0, 0) and the bottom right is (1, 1).
	* @param maxLineLength
	*            - basically the width of the virtual page in terms of screen
	*            width (1 is full screen width, 0.5 is half the width of the
	*            screen, etc.) Text cannot go off the edge of the page, so if
	*            the text is longer than this length it will go onto the next
	*            line. When text is centered it is centered into the middle of
	*            the line, based on this line length value.
	* @param centered
	*            - whether the text should be centered or not.
	*/
public:
	GUIText(const std::string& text, float fontSize, const FontType& font, const glm::vec2& position, float maxLineLength,
		bool centered) :
		m_textString { text },
		m_fontSize { fontSize },
		m_font { font },
		m_position { position },
		m_lineMaxSize { maxLineLength },
		m_centerText { centered }
		// load text
	{
		TextMaster::loadText(*this);
	}

	/**
	* Remove the text from the screen.
	*/
	void remove()
	{
		TextMaster::removeText(*this);
	}

	/**
	* @return The font used by this text.
	*/
	FontType getFont()
	{
		return m_font;
	}

	/**
	* Set the colour of the text.
	*
	* @param r
	*            - red value, between 0 and 1.
	* @param g
	*            - green value, between 0 and 1.
	* @param b
	*            - blue value, between 0 and 1.
	*/
	void setColour(float r, float g, float b)
	{
		m_colour = { r, g, b };
	}

	/**
	* @return the colour of the text.
	*/
	glm::vec3 getColour()
	{
		return m_colour;
	}

	/**
	* @return The number of lines of text. This is determined when the text is
	*         loaded, based on the length of the text and the max line length
	*         that is set.
	*/
	int getNumberOfLines()
	{
		return m_numberOfLines;
	}

	/**
	* @return The position of the top-left corner of the text in screen-space.
	*         (0, 0) is the top left corner of the screen, (1, 1) is the bottom
	*         right.
	*/
	glm::vec2 getPosition()
	{
		return m_position;
	}

	/**
	* @return the ID of the text's VAO, which contains all the vertex data for
	*         the quads on which the text will be rendered.
	*/
	int getMesh()
	{
		return m_textMeshVao;
	}

	/**
	* Set the VAO and vertex count for this text.
	*
	* @param vao
	*            - the VAO containing all the vertex data for the quads on
	*            which the text will be rendered.
	* @param verticesCount
	*            - the total number of vertices in all of the quads.
	*/
	void setMeshInfo(int vao, int verticesCount)
	{
		m_textMeshVao = vao;
		m_vertexCount = verticesCount;
	}

	/**
	* @return The total number of vertices of all the text's quads.
	*/
	int getVertexCount()
	{
		return m_vertexCount;
	}

	/**
	* @return the font size of the text (a font size of 1 is normal).
	*/
	float getFontSize()
	{
		return m_fontSize;
	}

	/**
	* Sets the number of lines that this text covers (method used only in
	* loading).
	*
	* @param number
	*/
	void setNumberOfLines(int number)
	{
		m_numberOfLines = number;
	}

	/**
	* @return {@code true} if the text should be centered.
	*/
	bool isCentered()
	{
		return m_centerText;
	}

	/**
	* @return The maximum length of a line of this text.
	*/
	float getMaxLineSize()
	{
		return m_lineMaxSize;
	}

	/**
	* @return The string of text.
	*/
	std::string getTextString()
	{
		return m_textString;
	}
};
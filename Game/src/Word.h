#pragma once

#include "Character.h"

#include <vector>

class Word
{
private:
	std::vector<Character> m_characters;
	double m_width = 0;
	double m_fontSize;

	/**
	* Create a new empty word.
	* @param fontSize - the font size of the text which this word is in.
	*/
public:
	Word(double fontSize)
	{
		fontSize = fontSize;
	}

	/**
	* Adds a character to the end of the current word and increases the screen-space width of the word.
	* @param character - the character to be added.
	*/
	void addCharacter(Character character)
	{
		m_characters.push_back(character);
		m_width += character.getxAdvance() * m_fontSize;
	}

	/**
	* @return The list of characters in the word.
	*/
	std::vector<Character> getCharacters()
	{
		return m_characters;
	}

	/**
	* @return The width of the word in terms of screen size.
	*/
	double getWordWidth()
	{
		return m_width;
	}

};
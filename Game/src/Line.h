#pragma once

#include <vector>

#include "Word.h"

class Line
{
private:
	double maxLength;
	double spaceSize;

	std::vector<Word> words;
	double currentLineLength = 0;

	/**
	* Creates an empty line.
	*
	* @param spaceWidth
	*            - the screen-space width of a space character.
	* @param fontSize
	*            - the size of font being used.
	* @param maxLength
	*            - the screen-space maximum length of a line.
	*/
public:
	Line(double spaceWidth, double fontSize, double maxLength) :
		spaceSize { spaceWidth * fontSize },
		maxLength { maxLength }
	{ }

	/**
	* Attempt to add a word to the line. If the line can fit the word in
	* without reaching the maximum line length then the word is added and the
	* line length increased.
	*
	* @param word
	*            - the word to try to add.
	* @return {@code true} if the word has successfully been added to the line.
	*/
	bool attemptToAddWord(Word word)
	{
		double additionalLength = word.getWordWidth();
		additionalLength += !words.empty() ? spaceSize : 0;
		if (currentLineLength + additionalLength <= maxLength)
		{
			words.push_back(word);
			currentLineLength += additionalLength;
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	* @return The max length of the line.
	*/
	double getMaxLength()
	{
		return maxLength;
	}

	/**
	* @return The current screen-space length of the line.
	*/
	double getLineLength()
	{
		return currentLineLength;
	}

	/**
	* @return The list of words in the line.
	*/
	std::vector<Word> getWords()
	{
		return words;
	}
};
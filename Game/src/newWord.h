#pragma once

#include <string>
#include <vector>

#include "newCharacter.h"

class Word
{
private:
	std::vector<Character> m_characters;
	double m_width = 0;
public:
	Word(const std::vector<Character> characters) : m_characters(characters)
	{ }
};
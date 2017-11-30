#pragma once

#include "MetaFile.h"
#include "Line.h"

#include <memory>

class TextMeshCreator
{
private:
	static std::shared_ptr<MetaFile> metaData;
public:
	const static double LINE_HEIGHT;
	const static int SPACE_ASCII;
public:
	TextMeshCreator(const std::string& metaFile) : metaData { std::make_shared<MetaFile>(metaFile) }
	{ }

	TextMeshData createTextMesh(GUIText& text)
	{
		std::vector<Line> lines = createStructure(text);
		TextMeshData data = createQuadVertices(text, lines);
		return data;
	}
private:
	std::vector<Line> createStructure(GUIText& text)
	{
		std::string chars = text.getTextString();
		std::vector<Line> lines;
		Line currentLine(metaData->getSpaceWidth(), text.getFontSize(), text.getMaxLineSize());
		Word currentWord(text.getFontSize());
		for (char c : chars)
		{
			int ascii = (int) c;
			if (ascii == SPACE_ASCII)
			{
				bool added = currentLine.attemptToAddWord(currentWord);
				if (!added)
				{
					lines.push_back(currentLine);
					currentLine = Line(metaData->getSpaceWidth(), text.getFontSize(), text.getMaxLineSize());
					currentLine.attemptToAddWord(currentWord);
				}
				currentWord = Word(text.getFontSize());
				continue;
			}
			Character character = metaData->getCharacter(ascii);
			currentWord.addCharacter(character);
		}
		completeStructure(lines, currentLine, currentWord, text);
		return lines;
	}

	void completeStructure(std::vector<Line>& lines, Line& currentLine, Word& currentWord, GUIText& text)
	{
		bool added = currentLine.attemptToAddWord(currentWord);
		if (!added)
		{
			lines.push_back(currentLine);
			currentLine = Line(metaData->getSpaceWidth(), text.getFontSize(), text.getMaxLineSize());
			currentLine.attemptToAddWord(currentWord);
		}
		lines.push_back(currentLine);
	}

	TextMeshData createQuadVertices(GUIText& text, std::vector<Line> lines)
	{
		text.setNumberOfLines(lines.size());
		double curserX = 0.0f;
		double curserY = 0.0f;
		std::vector<float> vertices;
		std::vector<float> textureCoords;
		for (Line line : lines)
		{
			if (text.isCentered())
			{
				curserX = (line.getMaxLength() - line.getLineLength()) / 2;
			}
			for (Word word : line.getWords())
			{
				for (Character letter : word.getCharacters())
				{
					addVerticesForCharacter(curserX, curserY, letter, text.getFontSize(), vertices);
					addTexCoords(textureCoords, letter.getxTextureCoord(), letter.getyTextureCoord(),
						letter.getXMaxTextureCoord(), letter.getYMaxTextureCoord());
					curserX += letter.getxAdvance() * text.getFontSize();
				}
				curserX += metaData->getSpaceWidth() * text.getFontSize();
			}
			curserX = 0;
			curserY += LINE_HEIGHT * text.getFontSize();
		}
		return TextMeshData(vertices, textureCoords);
	}

	void addVerticesForCharacter(double curserX, double curserY, Character character, double fontSize,
		std::vector<float> vertices)
	{
		double x = curserX + (character.getxOffset() * fontSize);
		double y = curserY + (character.getyOffset() * fontSize);
		double maxX = x + (character.getSizeX() * fontSize);
		double maxY = y + (character.getSizeY() * fontSize);
		double properX = (2 * x) - 1;
		double properY = (-2 * y) + 1;
		double properMaxX = (2 * maxX) - 1;
		double properMaxY = (-2 * maxY) + 1;
		addVertices(vertices, properX, properY, properMaxX, properMaxY);
	}

	void addVertices(std::vector<float> vertices, double x, double y, double maxX, double maxY)
	{
		vertices.push_back((float) x);
		vertices.push_back((float) y);
		vertices.push_back((float) x);
		vertices.push_back((float) maxY);
		vertices.push_back((float) maxX);
		vertices.push_back((float) maxY);
		vertices.push_back((float) maxX);
		vertices.push_back((float) maxY);
		vertices.push_back((float) maxX);
		vertices.push_back((float) y);
		vertices.push_back((float) x);
		vertices.push_back((float) y);
	}

	void addTexCoords(std::vector<float> texCoords, double x, double y, double maxX, double maxY)
	{
		texCoords.push_back((float) x);
		texCoords.push_back((float) y);
		texCoords.push_back((float) x);
		texCoords.push_back((float) maxY);
		texCoords.push_back((float) maxX);
		texCoords.push_back((float) maxY);
		texCoords.push_back((float) maxX);
		texCoords.push_back((float) maxY);
		texCoords.push_back((float) maxX);
		texCoords.push_back((float) y);
		texCoords.push_back((float) x);
		texCoords.push_back((float) y);
	}
};

const double TextMeshCreator::LINE_HEIGHT = 0.03f;
const int TextMeshCreator::SPACE_ASCII = 32;
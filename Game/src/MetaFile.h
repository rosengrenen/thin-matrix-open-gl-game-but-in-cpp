#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Character.h"
#include "TextMeshCreator.h"
#include "Utilities.h"
#include "Window.h"

class MetaFile
{
private:
	const int PAD_TOP = 0;
	const int PAD_LEFT = 1;
	const int PAD_BOTTOM = 2;
	const int PAD_RIGHT = 3;
	const int DESIRED_PADDING = 3;
	const char SPLITTER = ' ';
	const char NUMBER_SEPARATOR = ',';

	double aspectRatio = Window::getAspectRatio();

	double verticalPerPixelSize;
	double horizontalPerPixelSize;
	double spaceWidth;
	std::vector<int> padding;
	int paddingWidth;
	int paddingHeight;

	std::unordered_map<int, Character> metaData;
	std::unordered_map<std::string, std::string> values;

public:
	/**
	* Opens a font file in preparation for reading.
	*
	* @param file
	*            - the font file.
	*/
	MetaFile(const std::string& path)
	{
		std::ifstream stream(path);
		loadPaddingData(stream);
		loadLineSizes(stream);
		int imageWidth = getValueOfVariable("scaleW");
		loadCharacterData(imageWidth, stream);
	}

	double getSpaceWidth()
	{
		return spaceWidth;
	}

	Character getCharacter(int ascii)
	{
		return metaData.at(ascii);
	}

	/**
	* Read in the next line and store the variable values.
	*
	* @return {@code true} if the end of the file hasn't been reached.
	*/
private:
	bool processNextLine(std::ifstream& stream)
	{
		values.clear();
		std::string line;
		getline(stream, line);
		for (auto& part : Utilities::stringExplode(line, SPLITTER))
		{
			std::vector<std::string> valuePairs { Utilities::stringExplode(part, '=') };
			if (valuePairs.size() == 2)
			{
				values.insert(valuePairs.at(0), valuePairs.at(1));
			}
		}
		return true;
	}

	/**
	* Gets the {@code int} value of the variable with a certain name on the
	* current line.
	*
	* @param variable
	*            - the name of the variable.
	* @return The value of the variable.
	*/
	int getValueOfVariable(const std::string& variable)
	{
		return ::atoi(values.at(variable).c_str());
	}

	/**
	* Gets the array of ints associated with a variable on the current line.
	*
	* @param variable
	*            - the name of the variable.
	* @return The int array of values associated with the variable.
	*/
	std::vector<int> getValuesOfVariable(const std::string& variable)
	{
		std::vector<std::string> numbers { Utilities::stringExplode(values.at(variable), NUMBER_SEPARATOR) };
		std::vector<int> actualValues(numbers.size());
		for (int i = 0; i < actualValues.size(); i++)
		{
			actualValues.at(i) = ::atoi(numbers[i].c_str());
		}
		return actualValues;
	}

	/**
	* Loads the data about how much padding is used around each character in
	* the texture atlas.
	*/
	void loadPaddingData(std::ifstream& stream)
	{
		processNextLine(stream);
		padding = getValuesOfVariable("padding");
		paddingWidth = padding[PAD_LEFT] + padding[PAD_RIGHT];
		paddingHeight = padding[PAD_TOP] + padding[PAD_BOTTOM];
	}

	/**
	* Loads information about the line height for this font in pixels, and uses
	* this as a way to find the conversion rate between pixels in the texture
	* atlas and screen-space.
	*/
	void loadLineSizes(std::ifstream& stream)
	{
		processNextLine(stream);
		int lineHeightPixels = getValueOfVariable("lineHeight") - paddingHeight;
		verticalPerPixelSize = TextMeshCreator::LINE_HEIGHT / (double) lineHeightPixels;
		horizontalPerPixelSize = verticalPerPixelSize / aspectRatio;
	}

	/**
	* Loads in data about each character and stores the data in the
	* {@link Character} class.
	*
	* @param imageWidth
	*            - the width of the texture atlas in pixels.
	*/
	void loadCharacterData(int imageWidth, std::ifstream& stream)
	{
		processNextLine(stream);
		processNextLine(stream);
		while (processNextLine(stream))
		{
			std::unique_ptr<Character> c = loadCharacter(imageWidth);
			if (!c)
			{
				metaData.insert({ c->getId(), *c });
			}
		}
	}

	/**
	* Loads all the data about one character in the texture atlas and converts
	* it all from 'pixels' to 'screen-space' before storing. The effects of
	* padding are also removed from the data.
	*
	* @param imageSize
	*            - the size of the texture atlas in pixels.
	* @return The data about the character.
	*/
	std::unique_ptr<Character> loadCharacter(int imageSize)
	{
		int id = getValueOfVariable("id");
		if (id == TextMeshCreator::SPACE_ASCII)
		{
			spaceWidth = (getValueOfVariable("xadvance") - paddingWidth) * horizontalPerPixelSize;
			return nullptr;
		}
		double xTex = ((double) getValueOfVariable("x") + (padding[PAD_LEFT] - DESIRED_PADDING)) / imageSize;
		double yTex = ((double) getValueOfVariable("y") + (padding[PAD_TOP] - DESIRED_PADDING)) / imageSize;
		int width = getValueOfVariable("width") - (paddingWidth - (2 * DESIRED_PADDING));
		int height = getValueOfVariable("height") - ((paddingHeight) -(2 * DESIRED_PADDING));
		double quadWidth = width * horizontalPerPixelSize;
		double quadHeight = height * verticalPerPixelSize;
		double xTexSize = (double) width / imageSize;
		double yTexSize = (double) height / imageSize;
		double xOff = (getValueOfVariable("xoffset") + padding[PAD_LEFT] - DESIRED_PADDING) * horizontalPerPixelSize;
		double yOff = (getValueOfVariable("yoffset") + (padding[PAD_TOP] - DESIRED_PADDING)) * verticalPerPixelSize;
		double xAdvance = (getValueOfVariable("xadvance") - paddingWidth) * horizontalPerPixelSize;
		return std::make_unique<Character>(Character(id, xTex, yTex, xTexSize, yTexSize, xOff, yOff, quadWidth, quadHeight, xAdvance));
	}
};
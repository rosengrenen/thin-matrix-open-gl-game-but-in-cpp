#pragma once

class Character
{
private:
	int m_id;
	double m_xTextureCoord;
	double m_yTextureCoord;
	double m_xMaxTextureCoord;
	double m_yMaxTextureCoord;
	double m_xOffset;
	double m_yOffset;
	double m_sizeX;
	double m_sizeY;
	double m_xAdvance;

	/**
	* @param id
	*            - the ASCII value of the character.
	* @param xTextureCoord
	*            - the x texture coordinate for the top left corner of the
	*            character in the texture atlas.
	* @param yTextureCoord
	*            - the y texture coordinate for the top left corner of the
	*            character in the texture atlas.
	* @param xTexSize
	*            - the width of the character in the texture atlas.
	* @param yTexSize
	*            - the height of the character in the texture atlas.
	* @param xOffset
	*            - the x distance from the curser to the left edge of the
	*            character's quad.
	* @param yOffset
	*            - the y distance from the curser to the top edge of the
	*            character's quad.
	* @param sizeX
	*            - the width of the character's quad in screen space.
	* @param sizeY
	*            - the height of the character's quad in screen space.
	* @param xAdvance
	*            - how far in pixels the cursor should advance after adding
	*            this character.
	*/
public:
	Character(int id, double xTextureCoord, double yTextureCoord, double xTexSize, double yTexSize,
		double xOffset, double yOffset, double sizeX, double sizeY, double xAdvance) :
		m_id { id },
		m_xTextureCoord { xTextureCoord },
		m_yTextureCoord { yTextureCoord },
		m_xOffset { xOffset },
		m_yOffset { yOffset },
		m_sizeX { sizeX },
		m_sizeY { sizeY },
		m_xMaxTextureCoord { xTexSize + xTextureCoord },
		m_yMaxTextureCoord { yTexSize + yTextureCoord },
		m_xAdvance { xAdvance }
	{ }

	int getId()
	{
		return m_id;
	}

	double getxTextureCoord()
	{
		return m_xTextureCoord;
	}

	double getyTextureCoord()
	{
		return m_yTextureCoord;
	}

	double getXMaxTextureCoord()
	{
		return m_xMaxTextureCoord;
	}

	double getYMaxTextureCoord()
	{
		return m_yMaxTextureCoord;
	}

	double getxOffset()
	{
		return m_xOffset;
	}

	double getyOffset()
	{
		return m_yOffset;
	}

	double getSizeX()
	{
		return m_sizeX;
	}

	double getSizeY()
	{
		return m_sizeY;
	}

	double getxAdvance()
	{
		return m_xAdvance;
	}
};
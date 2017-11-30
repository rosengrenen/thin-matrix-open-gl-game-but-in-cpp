#pragma once

#include <GL\glew.h>

#include <glm\mat2x2.hpp>
#include <glm\vec3.hpp>

#include "ShaderProgram.h"

class FontShader : public ShaderProgram
{
private:
	const std::string path = "src/fontRendering/fontVertex.txt";
	GLuint m_translationLoc;
	GLuint m_colourLoc;
	GLuint m_fontAtlasLoc;
private:
	void getUniformLocations() override
	{
		m_translationLoc = getUniformLocation("translation");
		m_colourLoc = getUniformLocation("colour");
		m_fontAtlasLoc = getUniformLocation("fontAtlas");
		setInt(m_fontAtlasLoc, 0);
	}

	void bindAttribLocations() override
	{
		bindAttribLocation(0, "position");
		bindAttribLocation(1, "texCoord");
	}
public:
	FontShader()
	{
		ShaderProgram::createShaderProgram(path);
	}

	void setColour(const glm::vec3& colour)
	{
		setVector3f(m_colourLoc, colour);
	}

	void setTranslation(const glm::vec2& position)
	{
		setVector2f(m_translationLoc, position);
	}
};
#pragma once

#include "ShaderProgram.h"
#include <glm\mat4x4.hpp>

class GuiShader : public ShaderProgram
{
private:
	const std::string m_shaderPath = "res/shaders/gui.shader";
	unsigned int m_transformationMatrixLoc;
public:
	GuiShader()
	{
		ShaderProgram::createShaderProgram(m_shaderPath);
	}

	virtual void bindAttribLocations() override
	{
		bindAttribLocation(0, "position");
	}

	virtual void getUniformLocations() override
	{
		m_transformationMatrixLoc = getUniformLocation("transformation");
	}

	void setTransformationMatrix(const glm::mat4& matrix) const
	{
		setMatrix4f(m_transformationMatrixLoc, matrix);
	}
};
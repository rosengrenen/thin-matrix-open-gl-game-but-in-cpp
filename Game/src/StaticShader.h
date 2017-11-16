#pragma once

#include "BaseShader.h"

class StaticShader : public BaseShader
{
private:
	unsigned int m_transformationMatrixLoc;
	unsigned int m_projectionMatrixLoc;
	unsigned int m_viewMatrixLoc;
public:
	StaticShader(const std::string& path)
	{
		BaseShader::createShaderProgram(path);
	}

	virtual void bindAttribLocations() override
	{
		bindAttribLocation(0, "position");
		bindAttribLocation(1, "texCoords");
	}

	virtual void getUniformLocations() override
	{
		m_transformationMatrixLoc = getUniformLocation("transformationMatrix");
		m_projectionMatrixLoc = getUniformLocation("projectionMatrix");
		m_viewMatrixLoc = getUniformLocation("viewMatrix");
	}

	void setTransformationMatrix(glm::mat4 matrix)
	{
		setMatrix4x4(m_transformationMatrixLoc, matrix);
	}

	void setProjectionMatrix(glm::mat4 matrix)
	{
		setMatrix4x4(m_projectionMatrixLoc, matrix);
	}

	void setViewMatrix(glm::mat4 matrix)
	{
		setMatrix4x4(m_viewMatrixLoc, matrix);
	}
};
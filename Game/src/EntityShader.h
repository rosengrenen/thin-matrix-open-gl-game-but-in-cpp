#pragma once

#include "ShaderProgram.h"

class EntityShader : public ShaderProgram
{
private:
	const std::string m_shaderPath = "res/shaders/entity.shader";
	unsigned int m_modelMatrixLoc;
	unsigned int m_viewMatrixLoc;
	unsigned int m_projectionMatrixLoc;
	unsigned int m_lightPositionLoc;
	unsigned int m_lightColourLoc;
	unsigned int m_reflectivityLoc;
	unsigned int m_shineDamperLoc;
public:
	EntityShader()
	{
		ShaderProgram::createShaderProgram(m_shaderPath);
	}

	virtual void bindAttribLocations() override
	{
		bindAttribLocation(0, "position");
		bindAttribLocation(1, "texCoords");
		bindAttribLocation(2, "normal");
	}

	virtual void getUniformLocations() override
	{
		m_modelMatrixLoc = getUniformLocation("model");
		m_projectionMatrixLoc = getUniformLocation("projection");
		m_viewMatrixLoc = getUniformLocation("view");
		m_lightPositionLoc = getUniformLocation("lightPosition");
		m_lightColourLoc = getUniformLocation("lightColour");
		m_reflectivityLoc = getUniformLocation("reflectivity");
		m_shineDamperLoc = getUniformLocation("shineDamper");
	}

	void setModelMatrix(const glm::mat4& matrix) const
	{
		setMatrix4x4(m_modelMatrixLoc, matrix);
	}

	void setProjectionMatrix(const glm::mat4& matrix) const
	{
		setMatrix4x4(m_projectionMatrixLoc, matrix);
	}

	void setViewMatrix(const glm::mat4& matrix) const
	{
		setMatrix4x4(m_viewMatrixLoc, matrix);
	}

	void setLight(const Light& light) const
	{
		setVector3f(m_lightPositionLoc, light.position);
		setVector3f(m_lightColourLoc, light.colour);
	}

	void setShineVariables(float reflectivity, float damper)
	{
		setFloat(m_reflectivityLoc, reflectivity);
		setFloat(m_shineDamperLoc, damper);
	}
};
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
	unsigned int m_fakeNormalsLoc;
	unsigned int m_skyColourLoc;
	unsigned int m_atlasOffsetLoc;
	unsigned int m_numRowsLoc;
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
		m_fakeNormalsLoc = getUniformLocation("useFakeLighting");
		m_skyColourLoc = getUniformLocation("skyColour");
		m_numRowsLoc = getUniformLocation("numRows");
		m_atlasOffsetLoc = getUniformLocation("offset");
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

	void setFakeLighting(bool useFake)
	{
		setFloat(m_fakeNormalsLoc, useFake);
	}

	void setSkyColour(float r, float g, float b)
	{
		setVector3f(m_skyColourLoc, glm::vec3(r, g, b));
	}

	void setNumRows(int numRows)
	{
		setFloat(m_numRowsLoc, numRows);
	}

	void setAtlasOffset(float x, float y)
	{
		setVector2f(m_atlasOffsetLoc, glm::vec2(x, y));
	}
};
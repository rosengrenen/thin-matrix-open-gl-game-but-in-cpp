#pragma once

#include <glm\mat4x4.hpp>

#include "Light.h"
#include "ShaderProgram.h"

class TerrainShader : public ShaderProgram
{
private:
	const std::string m_shaderPath = "res/shaders/terrain.shader";
	unsigned int m_modelMatrixLoc;
	unsigned int m_viewMatrixLoc;
	unsigned int m_projectionMatrixLoc;
	unsigned int m_lightPositionLoc;
	unsigned int m_lightColourLoc;
	unsigned int m_reflectivityLoc;
	unsigned int m_shineDamperLoc;
	unsigned int m_skyColourLoc;
	unsigned int m_bgTexLoc;
	unsigned int m_rTexLoc;
	unsigned int m_gTexLoc;
	unsigned int m_bTexLoc;
	unsigned int m_blendMapLoc;
public:
	TerrainShader()
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
		m_skyColourLoc = getUniformLocation("skyColour");
		m_bgTexLoc = getUniformLocation("backgroundTexture");
		m_rTexLoc = getUniformLocation("rTexture");
		m_gTexLoc = getUniformLocation("gTexture");
		m_bTexLoc = getUniformLocation("bTexture");
		m_blendMapLoc = getUniformLocation("blendMap");
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

	void setSkyColour(float r, float g, float b)
	{
		setVector3f(m_skyColourLoc, glm::vec3(r, g, b));
	}

	void connectTextureUnits()
	{
		setInt(m_bgTexLoc, 0);
		setInt(m_rTexLoc, 1);
		setInt(m_gTexLoc, 2);
		setInt(m_bTexLoc, 3);
		setInt(m_blendMapLoc, 4);
	}
};
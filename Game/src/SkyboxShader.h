#pragma once

#include <GL\glew.h>
#include <glm\mat4x4.hpp>
#include "ShaderProgram.h"

class SkyboxShader : public ShaderProgram
{
private:
	const std::string m_shaderPath = "res/shaders/skybox.shader";
	GLuint m_projectionMatrixLoc;
	GLuint m_viewMatrixLoc;
	GLuint m_fogColourLoc;
	GLuint m_blendFactorLoc;
	GLuint m_cubeMapLoc;
	GLuint m_cubeMapLoc2;
public:
	SkyboxShader()
	{
		ShaderProgram::createShaderProgram(m_shaderPath);
	}

	void bindAttribLocations() override
	{
		bindAttribLocation(0, "position");
	}

	void getUniformLocations() override
	{
		m_projectionMatrixLoc = getUniformLocation("projection");
		m_viewMatrixLoc = getUniformLocation("view");
		m_fogColourLoc = getUniformLocation("fogColour");
		m_blendFactorLoc = getUniformLocation("blendFactor");
		m_cubeMapLoc = getUniformLocation("cubeMap");
		m_cubeMapLoc2 = getUniformLocation("cubeMap2");
		setInt(m_cubeMapLoc, 0);
		setInt(m_cubeMapLoc2, 1);
	}

	void setProjectionMatrix(const glm::mat4& projection)
	{
		setMatrix4x4(m_projectionMatrixLoc, projection);
	}

	void setViewMatrix(glm::mat4& view)
	{
		// Remove translations so that the sky doesnt move in relation to the camera
		view[3][0] = 0;
		view[3][1] = 0;
		view[3][2] = 0;
		setMatrix4x4(m_viewMatrixLoc, view);
	}

	void setFogColour(float r, float g, float b)
	{
		setVector3f(m_fogColourLoc, { r, g, b });
	}

	void setBlendFactor(float f)
	{
		setFloat(m_blendFactorLoc, f);
	}
};
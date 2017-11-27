#include "EntityShader.h"

EntityShader::EntityShader()
{
	ShaderProgram::createShaderProgram(m_shaderPath);
}

void EntityShader::bindAttribLocations()
{
	bindAttribLocation(0, "position");
	bindAttribLocation(1, "texCoords");
	bindAttribLocation(2, "normal");
}

void EntityShader::getUniformLocations()
{
	m_modelMatrixLoc = getUniformLocation("model");
	m_projectionMatrixLoc = getUniformLocation("projection");
	m_viewMatrixLoc = getUniformLocation("view");
	m_reflectivityLoc = getUniformLocation("reflectivity");
	m_shineDamperLoc = getUniformLocation("shineDamper");
	m_fakeNormalsLoc = getUniformLocation("useFakeLighting");
	m_skyColourLoc = getUniformLocation("skyColour");
	m_numRowsLoc = getUniformLocation("numRows");
	m_atlasOffsetLoc = getUniformLocation("offset");
	m_planeLoc = getUniformLocation("plane");

	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		m_lightPositionLoc.push_back(getUniformLocation("lightPosition[" + std::to_string(i) + "]"));
		m_lightColourLoc.push_back(getUniformLocation("lightColour[" + std::to_string(i) + "]"));
		m_lightAttenuationLoc.push_back(getUniformLocation("attenuation[" + std::to_string(i) + "]"));
	}
}

void EntityShader::setModelMatrix(const glm::mat4& matrix) const
{
	setMatrix4f(m_modelMatrixLoc, matrix);
}

void EntityShader::setProjectionMatrix(const glm::mat4& matrix) const
{
	setMatrix4f(m_projectionMatrixLoc, matrix);
}

void EntityShader::setViewMatrix(const glm::mat4& matrix) const
{
	setMatrix4f(m_viewMatrixLoc, matrix);
}

void EntityShader::setLight(const std::vector<Light>& lights) const
{
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		if (i < lights.size())
		{
			setVector3f(m_lightPositionLoc[i], lights[i].position);
			setVector3f(m_lightColourLoc[i], lights[i].colour);
			setVector3f(m_lightAttenuationLoc[i], lights[i].attenuation);
		}
		else
		{
			setVector3f(m_lightPositionLoc[i], { 0.0f, 0.0f, 0.0f });
			setVector3f(m_lightColourLoc[i], { 0.0f, 0.0f, 0.0f });
			setVector3f(m_lightAttenuationLoc[i], { 1.0f,0.0f,0.0f });
		}
	}
}

void EntityShader::setShineVariables(float reflectivity, float damper)
{
	setFloat(m_reflectivityLoc, reflectivity);
	setFloat(m_shineDamperLoc, damper);
}

void EntityShader::setFakeLighting(bool useFake)
{
	setFloat(m_fakeNormalsLoc, useFake);
}

void EntityShader::setSkyColour(float r, float g, float b)
{
	setVector3f(m_skyColourLoc, glm::vec3(r, g, b));
}

void EntityShader::setNumRows(int numRows)
{
	setFloat(m_numRowsLoc, numRows);
}

void EntityShader::setAtlasOffset(float x, float y)
{
	setVector2f(m_atlasOffsetLoc, glm::vec2(x, y));
}

void EntityShader::setClipPlane(const glm::vec4& plane)
{
	setVector4f(m_planeLoc, plane);
}
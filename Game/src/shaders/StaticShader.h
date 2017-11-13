#ifndef STATIC_SHADER_H
#define STATIC_SHADER_H

#include "ShaderProgram.h"

class StaticShader : public ShaderProgram
{
private:
	int m_transformationMatrixLocation;
	int m_texSamplerLocation;
public:
	StaticShader(const std::string& filePath)
	{
		createProgram(filePath);
	}
	void getAllUniformLocations() override
	{
		m_transformationMatrixLocation = getUniformLocation("transformationMatrix");
		m_texSamplerLocation = getUniformLocation("texSampler");
	}
	void bindAttributes() override
	{
		bindAttribute(0, "position");
		bindAttribute(1, "texCoords");
	}
	void loadTransformationMatrix(glm::mat4 matrix)
	{
		setMatrix4(m_transformationMatrixLocation, matrix);
	}
};

#endif
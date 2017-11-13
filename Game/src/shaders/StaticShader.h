#ifndef STATIC_SHADER_H
#define STATIC_SHADER_H

#include "ShaderProgram.h"

class StaticShader : public ShaderProgram
{
private:
	int m_transformationMatrixLocation;
public:
	StaticShader(const std::string& filePath) : ShaderProgram(filePath)
	{ }
	void getAllUniformLocations() override
	{
		m_transformationMatrixLocation = getUniformLocation("transformationMatrix");
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
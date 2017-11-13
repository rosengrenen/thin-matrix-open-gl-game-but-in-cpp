#ifndef STATIC_SHADER_H
#define STATIC_SHADER_H

#include "ShaderProgram.h"

class StaticShader : public ShaderProgram
{
public:
	StaticShader(const std::string& filePath) : ShaderProgram(filePath)
	{
	}
	virtual void bindAttributes() override
	{
		bindAttribute(0, "position");
	}
};

#endif
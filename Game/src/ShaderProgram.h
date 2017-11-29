#pragma once

#include <GL\glew.h>

#include <glm\mat4x4.hpp>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>

class ShaderProgram
{
private:
	unsigned int m_program;
	unsigned int m_vs;
	unsigned int m_fs;
	std::string vsource;
	std::string fsource;
private:
	std::tuple<std::string, std::string> parseShaderSource(const std::string& filePath);

	unsigned int createShaderFromSource(unsigned int type, const std::string& source, const std::string& path);
public:
	void createShaderProgram(const std::string& shaderPath);

	virtual void bindAttribLocations() = 0;

	void bindAttribLocation(unsigned int location, const std::string& name);

	virtual void getUniformLocations() = 0;

	unsigned int getUniformLocation(const std::string& name);

	void use() const;

	void stop() const;

	void setMatrix4f(unsigned int location, const glm::mat4& matrix) const;

	void setVector4f(unsigned int location, const glm::vec4& vector) const;

	void setVector3f(unsigned int location, const glm::vec3& vector) const;

	void setVector2f(unsigned int location, const glm::vec2& vector) const;

	void setFloat(unsigned int location, float value) const;

	void setInt(unsigned int location, int value) const;
};


#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <tuple>

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

class ShaderProgram
{
private:
	enum class Type
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	unsigned int m_program;
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
protected:
	unsigned int compileShader(const unsigned int type, const std::string& source);
	std::tuple<std::string, std::string> parseShaderSource(const std::string& shaderPath);
	void createProgram(const std::string& shaderPath);
public:
	~ShaderProgram();
	virtual void getAllUniformLocations() = 0;
	virtual void bindAttributes() = 0;
	void bindAttribute(const int attrib, const std::string& varName);
	void use();
	void stop();
	int getUniformLocation(std::string uniformName) const;
	// Set uniforms
	void setBool(const int location, const bool value) const;
	void setFloat(const int location, const float value) const;
	void setVec3f(const int location, const glm::vec3 value) const;
	void setMatrix4(const int location, const glm::mat4 matrix) const;
};

#endif
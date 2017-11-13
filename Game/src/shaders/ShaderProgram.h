#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <tuple>

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
private:
	unsigned int CompileShader(const unsigned int type, const std::string& source);
	std::tuple<std::string, std::string> ParseShaderSource(const std::string& shaderPath);
protected:
public:
	ShaderProgram(const std::string& filePath);
	~ShaderProgram();
	virtual void bindAttributes()
	{ }
	void bindAttribute(const int attrib, const std::string& varName);
	void use();
	void stop();
};

#endif
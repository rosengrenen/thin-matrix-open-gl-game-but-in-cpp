#include <GL\glew.h>

#include <iostream>
#include <sstream>
#include <fstream>

#include <glm\gtc\type_ptr.hpp>

unsigned int compileShader(const unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

void ShaderProgram::createProgram(const std::string& shaderPath)
{
	std::tuple<std::string, std::string> shaderSources = parseShaderSource(shaderPath);
	m_program = glCreateProgram();
	m_vertexShader = compileShader(GL_VERTEX_SHADER, std::get<0>(shaderSources));
	m_fragmentShader = compileShader(GL_FRAGMENT_SHADER, std::get<1>(shaderSources));

	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
	bindAttributes();
	glLinkProgram(m_program);
	glValidateProgram(m_program);
	getAllUniformLocations();
}

ShaderProgram::~ShaderProgram()
{
	stop();
	glDetachShader(m_program, m_vertexShader);
	glDetachShader(m_program, m_fragmentShader);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
	glDeleteProgram(m_program);
}

void ShaderProgram::bindAttribute(const int attrib, const std::string& varName)
{
	glBindAttribLocation(m_program, attrib, varName.c_str());
}

void ShaderProgram::use()
{
	glUseProgram(m_program);
}

void ShaderProgram::stop()
{
	glUseProgram(0);
}

int ShaderProgram::getUniformLocation(std::string uniformName) const
{
	return glGetUniformLocation(m_program, uniformName.c_str());
}

void ShaderProgram::setMatrix4(const int location, const glm::mat4 matrix) const
{ 
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setBool(const int location, const bool value) const
{
	glUniform1i(location, value);
}

void ShaderProgram::setFloat(const int location, const float value) const
{
	glUniform1f(location, value);
}

void ShaderProgram::setVec3f(const int location, const glm::vec3 vector) const
{
	glUniform3f(location, vector.x, vector.y, vector.z);
}
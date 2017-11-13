#include <GL\glew.h>

#include <iostream>
#include <sstream>
#include <fstream>

#include "ShaderProgram.h"

std::tuple<std::string, std::string> ShaderProgram::ParseShaderSource(const std::string& filePath)
{
	std::ifstream stream;

	std::string line;
	std::stringstream ss[2];
	Type type = Type::NONE;
	stream.open(filePath);
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = Type::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = Type::FRAGMENT;
		}
		else
		{
			if (type == Type::NONE)
				std::cout << "Shader type not found" << std::endl;
			ss[(int) type] << line << '\n';
		}
	}
	return std::make_tuple<std::string, std::string>(ss[0].str(), ss[1].str());
}

unsigned int ShaderProgram::CompileShader(const unsigned int type, const std::string& source)
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

ShaderProgram::ShaderProgram(const std::string& shaderPath)
{
	std::tuple<std::string, std::string> shaderSources = ParseShaderSource(shaderPath);
	m_program = glCreateProgram();
	m_vertexShader = CompileShader(GL_VERTEX_SHADER, std::get<0>(shaderSources));
	m_fragmentShader = CompileShader(GL_FRAGMENT_SHADER, std::get<1>(shaderSources));

	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
	glLinkProgram(m_program);
	glValidateProgram(m_program);
	bindAttributes();
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
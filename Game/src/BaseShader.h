#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include <tuple>

class BaseShader
{
private:
	unsigned int m_program;
	unsigned int m_vs;
	unsigned int m_fs;
private:
	std::tuple<std::string, std::string> parseShaderSource(const std::string& filePath)
	{
		std::ifstream stream;

		std::string line;
		std::stringstream ss[2];
		int type = -1;
		stream.open(filePath);
		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = 0;
				else if (line.find("fragment") != std::string::npos)
					type = 1;
			}
			else
			{
				if (type == -1)
					std::cout << "Shader type not found" << std::endl;
				ss[(int) type] << line << '\n';
			}
		}
		return std::make_tuple<std::string, std::string>(ss[0].str(), ss[1].str());
	}

	unsigned int createShaderFromSource(unsigned int type, const std::string& source)
	{
		unsigned int shader = glCreateShader(type);
		const char* temp = source.c_str();
		glShaderSource(shader, 1, &temp, nullptr);
		glCompileShader(shader);

		int result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*) alloca(length * sizeof(char));
			glGetShaderInfoLog(shader, length, &length, message);
			std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(shader);
		}
		return shader;
	}
public:
	void createShaderProgram(const std::string& shaderPath)
	{
		m_program = glCreateProgram();

		std::tuple<std::string, std::string> shaderSource = parseShaderSource("res/shaders/triangle.shader");

		m_vs = createShaderFromSource(GL_VERTEX_SHADER, std::get<0>(shaderSource));
		m_fs = createShaderFromSource(GL_FRAGMENT_SHADER, std::get<1>(shaderSource));

		glAttachShader(m_program, m_vs);
		glAttachShader(m_program, m_fs);

		bindAttribLocations();

		glLinkProgram(m_program);
		glValidateProgram(m_program);

		getUniformLocations();
	}

	virtual void bindAttribLocations() = 0;

	void bindAttribLocation(unsigned int location, const std::string& name)
	{
		glBindAttribLocation(m_program, location, name.c_str());
	}

	virtual void getUniformLocations() = 0;

	unsigned int getUniformLocation(const std::string& name)
	{
		return glGetUniformLocation(m_program, name.c_str());
	}

	void use() const
	{
		glUseProgram(m_program);
	}

	void stop() const
	{
		glUseProgram(0);
	}

	void setMatrix4x4(unsigned int location, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	void setVector3f(unsigned int location, const glm::vec3& vector) const
	{
		glUniform3f(location, vector.x, vector.y, vector.z);
	}

	void setFloat(unsigned int location, float value)
	{
		glUniform1f(location, value);
	}
};


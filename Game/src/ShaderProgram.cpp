#include "ShaderProgram.h"

std::tuple<std::string, std::string> ShaderProgram::parseShaderSource(const std::string& filePath)
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
				std::cout << "[ERROR] Shader type not found for " << filePath.c_str() << std::endl;
			ss[(int) type] << line << '\n';
		}
	}
	return std::make_tuple<std::string, std::string>(ss[0].str(), ss[1].str());
}

unsigned int ShaderProgram::createShaderFromSource(unsigned int type, const std::string& source, const std::string& path)
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
		std::cout << "[ERROR] Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader for " << path << " with message: " << message << std::endl;
		glDeleteShader(shader);
	}
	return shader;
}
void ShaderProgram::createShaderProgram(const std::string& shaderPath)
{
	m_program = glCreateProgram();

	std::tuple<std::string, std::string> shaderSource = parseShaderSource(shaderPath);

	vsource = std::get<0>(shaderSource);
	fsource = std::get<1>(shaderSource);
	m_vs = createShaderFromSource(GL_VERTEX_SHADER, std::get<0>(shaderSource), shaderPath);
	m_fs = createShaderFromSource(GL_FRAGMENT_SHADER, std::get<1>(shaderSource), shaderPath);

	glAttachShader(m_program, m_vs);
	glAttachShader(m_program, m_fs);

	bindAttribLocations();

	glLinkProgram(m_program);
	glValidateProgram(m_program);

	glUseProgram(m_program);

	getUniformLocations();
}

void ShaderProgram::bindAttribLocation(unsigned int location, const std::string& name)
{
	glBindAttribLocation(m_program, location, name.c_str());
}

unsigned int ShaderProgram::getUniformLocation(const std::string& name)
{
	GLint loc = glGetUniformLocation(m_program, name.c_str());
	if (loc == -1)
	{
		__debugbreak();
	}
	return loc;
}

void ShaderProgram::use() const
{
	glUseProgram(m_program);
}

void ShaderProgram::stop() const
{
	glUseProgram(0);
}

void ShaderProgram::setMatrix4f(unsigned int location, const glm::mat4& matrix) const
{
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::setVector4f(unsigned int location, const glm::vec4& vector) const
{
	glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void ShaderProgram::setVector3f(unsigned int location, const glm::vec3& vector) const
{
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::setVector2f(unsigned int location, const glm::vec2& vector) const
{
	glUniform2f(location, vector.x, vector.y);
}

void ShaderProgram::setFloat(unsigned int location, float value)  const
{
	glUniform1f(location, value);
}

void ShaderProgram::setInt(unsigned int location, int value) const
{
	glUniform1i(location, value);
}

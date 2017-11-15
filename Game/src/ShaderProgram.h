
#include <fstream>
#include <iostream>
#include <sstream>

#include <tuple>

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
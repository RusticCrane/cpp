#include "Shader.h"

#include <fstream>
#include <iostream>

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile)
{
	std::string vertex = ReadFileContents(vertexFile);
	std::string fragment = ReadFileContents(fragmentFile);

	unsigned int vertexShader = CompileShader(vertex.c_str(), GL_VERTEX_SHADER);
	unsigned int fragmentShader = CompileShader(fragment.c_str(), GL_FRAGMENT_SHADER);

	m_id = LinkProgram(vertexShader, fragmentShader);
}

void Shader::use()
{
	glUseProgram(m_id);
}

std::string Shader::ReadFileContents(const std::string& path)
{
	std::string output;
	std::ifstream file(path);
	file.seekg(0, std::ios::end);
	output.resize(file.tellg());
	file.seekg(0);
	file.read(output.data(), output.size());
	return output;
}

unsigned int Shader::CompileShader(const char* source, GLenum type)
{
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		char log[512];
		glGetShaderInfoLog(id, 512, nullptr, log);
		std::cout << "Vertex shader error: " << log << std::endl;
	}
	return id;
}

unsigned int Shader::LinkProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId)
{
	unsigned int id = glCreateProgram();
	glAttachShader(id, vertexShaderId);
	glAttachShader(id, fragmentShaderId);
	glLinkProgram(id);

	int result;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (!result)
	{
		char log[512];
		glGetProgramInfoLog(id, 512, nullptr, log);
		std::cout << "Shader program error: " << log << std::endl;
	}
	return id;
}

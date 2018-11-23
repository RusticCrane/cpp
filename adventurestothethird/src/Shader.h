#pragma once

#include <string>

#include <glad/glad.h>

class Shader
{
public:
	Shader(const std::string& vertexFile, const std::string& fragmentFile);

	void use();

private:
	unsigned int m_id;

	std::string ReadFileContents(const std::string& path);

	unsigned int CompileShader(const char* source, GLenum type);

	unsigned int LinkProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId);
};

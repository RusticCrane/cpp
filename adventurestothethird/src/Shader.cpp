#include "Shader.h"

#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile)
{
    std::string vertex = ReadFileContents(vertexFile);
    std::string fragment = ReadFileContents(fragmentFile);

    unsigned int vertexShader = CompileShader(vertex.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader = CompileShader(fragment.c_str(), GL_FRAGMENT_SHADER);

    m_id = LinkProgram(vertexShader, fragmentShader);
}

void Shader::Use() const
{
    glUseProgram(m_id);
}

void Shader::Stop() const
{
	glUseProgram(0);
}

void Shader::SetFloat(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& vector)
{
    SetVec3(name, vector.x, vector.y, vector.z);
}

void Shader::SetVec3(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_uniformLocations.find(name) != m_uniformLocations.end())
    {
        return m_uniformLocations[name];
    }
    int location = glGetUniformLocation(m_id, name.c_str());
    if (location == -1)
    {
		std::cout << "Warning: unform '" << name << "' doesn't exist!" << std::endl;
    }
	m_uniformLocations.insert({ name, location });
	return location;
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

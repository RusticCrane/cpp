#pragma once

#include <glad/glad.h>

#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

class Shader
{
public:
    Shader(const std::string& vertexFile, const std::string& fragmentFile);
    void Use() const;
	void Stop() const;
	void SetFloat(const std::string& name, float value);
	void SetVec3(const std::string& name, const glm::vec3& vector);
	void SetVec3(const std::string& name, float v0, float v1, float v2);
	void SetMat4(const std::string& name, const glm::mat4& matrix);
private:
    unsigned int m_id;

	int GetUniformLocation(const std::string& name);
    std::string ReadFileContents(const std::string& path);
    unsigned int CompileShader(const char* source, GLenum type);
    unsigned int LinkProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId);
	std::unordered_map<std::string, int> m_uniformLocations;
};

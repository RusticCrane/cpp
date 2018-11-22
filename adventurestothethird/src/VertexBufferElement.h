#pragma once

#include <string>

#include <glad/glad.h>

class VertexBufferElement
{
public:
	unsigned int count;
	GLenum type;
	std::string name;
};

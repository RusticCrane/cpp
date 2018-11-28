#include "VertexBufferElement.h"

#include <glad/glad.h>

#include <exception>

size_t VertexBufferElement::SizeOfType(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT:
		return sizeof(float);
	default:
		throw std::exception("Unknown type");
	}
}

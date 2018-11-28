#pragma once

class VertexBufferElement
{
public:
	unsigned int type;
	unsigned int count;

	static size_t SizeOfType(unsigned int type);
};
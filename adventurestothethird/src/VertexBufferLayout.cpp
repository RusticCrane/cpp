#include "VertexBufferLayout.h"

#include "VertexBufferElement.h"

const std::vector<VertexBufferElement>& VertexBufferLayout::GetElements() const
{
	return m_elements;
}

unsigned int VertexBufferLayout::GetStride() const
{
	return m_stride;
}
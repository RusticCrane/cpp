#pragma once

#include "VertexBufferLayout.h"

template <>
void VertexBufferLayout::Push<float>(int count, const std::string& name)
{
	m_elements.push_back({ count, GL_FLOAT, name });
}

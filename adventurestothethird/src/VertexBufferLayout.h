#pragma once

#include <string>
#include <vector>

#include "VertexBufferElement.h"

class VertexBufferLayout
{
public:
	template <typename T>
	void Push(int count, const std::string& name)
	{
		static_assert(false);
	}

	template <>
	void Push<float>(int count, const std::string& name);

private:
	std::vector<VertexBufferElement> m_elements;
};

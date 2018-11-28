#pragma once

#include <vector>

#include <glad/glad.h>

#include "VertexBufferElement.h"

class VertexBufferLayout
{
public:
    template<typename T>
    void Push(unsigned int amount)
    {
        static_assert(false);
    }

    template<>
    void Push<float>(unsigned int amount)
    {
        m_elements.push_back({ GL_FLOAT, amount });
        m_stride += amount * VertexBufferElement::SizeOfType(GL_FLOAT);
    }

    const std::vector<VertexBufferElement>& GetElements() const;
    unsigned int GetStride() const;
private:
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride = 0;
};

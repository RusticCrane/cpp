#pragma once

#include <vector>

#include <glad/glad.h>

class VertexBufferElement
{
public:
    unsigned int type;
    unsigned int count;

    static size_t SizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:
                return sizeof(float);
            default:
				throw std::exception("Unknown type");
        }
    }
};

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

    auto& GetElements() const
    {
		return m_elements;
    }

    auto GetStride() const
    {
		return m_stride;
    }

private:
    std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride = 0;
};

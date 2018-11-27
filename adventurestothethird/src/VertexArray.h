#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
    VertexArray();
    void AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;
private:
    unsigned int m_id;
};

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
}

void VertexArray::AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout)
{
    Bind();
    buffer.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i)
    {
        const auto& element = elements.at(i);
        glVertexAttribPointer(i, element.count, element.type, GL_FALSE, layout.GetStride(), (void *) offset);
		glEnableVertexAttribArray(i);
		offset += element.count * VertexBufferElement::SizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_id);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

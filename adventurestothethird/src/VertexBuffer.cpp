#include "VertexBuffer.h"

#include <glad/glad.h>

VertexBuffer::VertexBuffer(const void* data, unsigned size)
{
    glGenBuffers(1, &m_id);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

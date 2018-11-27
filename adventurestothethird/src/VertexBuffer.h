#pragma once
class VertexBuffer
{
public:
    VertexBuffer(const void* data, unsigned int size);
    void Bind() const;
    void Unbind() const;
private:
    unsigned int m_id;
};

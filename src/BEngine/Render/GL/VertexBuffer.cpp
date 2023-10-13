#include "BEngine/Render/GL/VertexBuffer.hpp"

#include <cstddef>

VertexBuffer::VertexBuffer(const void *data, const std::size_t size)
{
    GL_Call(glGenBuffers(1, &m_Id));
    Bind();
    GL_Call(glBufferData(GL_ARRAY_BUFFER, static_cast<long>(size), data, GL_STATIC_DRAW));
}

void VertexBuffer::Bind() const
{
    GL_Call(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
}

void VertexBuffer::Unbind() const
{
    GL_Call(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

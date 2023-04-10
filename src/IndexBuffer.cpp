#include <cstdint>

#include <GL/glew.h>

#include "glsandbox/GLUtils.hpp"
#include "glsandbox/IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const uint32_t *indices, const uint64_t count)
    : m_Count(count)
{
    GL_Call(glGenBuffers(1, &m_Id));
    Bind();
    GL_Call(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         static_cast<long>(count * sizeof(uint32_t)), indices,
                         GL_STATIC_DRAW));
}

void IndexBuffer::Bind() const
{
    GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
}

void IndexBuffer::Unbind() const
{
    GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}


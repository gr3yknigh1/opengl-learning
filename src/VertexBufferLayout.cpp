#include "glsandbox/VertexBufferLayout.hpp"

#include <GL/glew.h>

VertexBufferLayout::VertexBufferLayout() : m_Stride(0)
{
}

const std::vector<VertexBufferAttribute> VertexBufferLayout::GetAttributes()
    const
{
    return m_Attributes;
}

std::size_t VertexBufferLayout::GetStride() const
{
    return m_Stride;
}

// TODO: Implement size
void VertexBufferLayout::Pushf(const uint32_t count)
{
    const auto size = sizeof(GLfloat);
    m_Attributes.push_back(VertexBufferAttribute(GL_FLOAT, count, size, false));
    m_Stride += size * count;
}

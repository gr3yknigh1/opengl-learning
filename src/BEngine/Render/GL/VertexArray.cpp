#include "VertexArray.hpp"

#include <cstdint>

#include "BEngine/Render/GL/Utils.hpp"

VertexArray::VertexArray()
{
    GL_Call(glGenVertexArrays(1, &m_Id));
}

void VertexArray::Bind() const
{
    GL_Call(glBindVertexArray(m_Id));
}

void VertexArray::Unbind() const
{
    GL_Call(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer &vb,
                            const VertexBufferLayout &layout)
{
    Bind();
    vb.Bind();
    uint64_t offset = 0;
    const auto &attributes = layout.GetAttributes();

    for (uint32_t i = 0; i < attributes.size(); i++)
    {
        const auto &attribute = attributes[i];

        GL_Call(glEnableVertexAttribArray(i));
        GL_Call(glVertexAttribPointer(i, static_cast<int>(attribute.count),
                                      attribute.type, attribute.isNormalized,
                                      static_cast<int>(layout.GetStride()),
                                      reinterpret_cast<const void *>(offset)));
        offset += static_cast<uint64_t>(attribute.count) * attribute.size;
    }
}

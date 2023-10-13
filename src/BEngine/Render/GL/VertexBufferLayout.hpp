#ifndef BENGINE_RENDER_GL_VERTEXBUFFERLAYOUT_HPP
#define BENGINE_RENDER_GL_VERTEXBUFFERLAYOUT_HPP

#include <vector>

#include "BEngine/Render/GL/VertexBufferAttribute.hpp"

class VertexBufferLayout
{
public:
    VertexBufferLayout();

    const std::vector<VertexBufferAttribute> GetAttributes() const;

    std::size_t GetStride() const;

    void PushFloat(const uint32_t count);

private:
    std::vector<VertexBufferAttribute> m_Attributes;
    std::size_t m_Stride;
};

#endif // BENGINE_RENDER_GL_VERTEXBUFFERLAYOUT_HPP

#ifndef VERTEXBUFFERLAYOUT_HPP_
#define VERTEXBUFFERLAYOUT_HPP_

#include <vector>

#include "glsandbox/VertexBufferAttribute.hpp"

class VertexBufferLayout
{
public:
    VertexBufferLayout();

    const std::vector<VertexBufferAttribute> GetAttributes() const;

    std::size_t GetStride() const;

    void Pushf(const uint32_t count);

private:
    std::vector<VertexBufferAttribute> m_Attributes;
    std::size_t m_Stride;
};

#endif // VERTEXBUFFERLAYOUTHPP_

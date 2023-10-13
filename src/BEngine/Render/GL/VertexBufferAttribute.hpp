#ifndef BENGINE_RENDER_GL_VERTEXBUFFERATTRIBUTE_HPP
#define BENGINE_RENDER_GL_VERTEXBUFFERATTRIBUTE_HPP

#include <cstddef>
#include <cstdint>

struct VertexBufferAttribute
{
    const uint32_t type;
    const uint32_t count;
    const std::size_t size;
    const bool isNormalized;

    VertexBufferAttribute(uint32_t _type, uint32_t _count, std::size_t _size, bool _isNormalized)
        : type(_type), count(_count), size(_size), isNormalized(_isNormalized)
    {
    }
};

#endif // BENGINE_RENDER_GL_VERTEXBUFFERATTRIBUTE_HPP

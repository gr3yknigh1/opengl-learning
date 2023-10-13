#ifndef BENGINE_RENDER_GL_INDEXBUFFER_HPP
#define BENGINE_RENDER_GL_INDEXBUFFER_HPP

#include <cstdint>
#include <vector>

#include "BEngine/Render/GL/GLObject.hpp"

class IndexBuffer : public GLObject
{
public:
    explicit IndexBuffer(const uint32_t *indices, const uint64_t count);
    explicit IndexBuffer(const std::vector<uint32_t> &indices);

    void Bind(void) const;
    void Unbind(void) const;

    constexpr uint64_t GetCount(void) const
    {
        return m_Count;
    }

private:
    const uint64_t m_Count;
};

#endif // BENGINE_RENDER_GL_INDEXBUFFER_HPP

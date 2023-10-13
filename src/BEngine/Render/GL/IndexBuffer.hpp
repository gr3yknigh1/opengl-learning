#ifndef INDEXBUFFER_HPP_
#define INDEXBUFFER_HPP_

#include <cstdint>
#include <vector>

#include "BEngine/Render/GL/GLObject.hpp"

class IndexBuffer : public GLObject
{
public:
    explicit IndexBuffer(const uint32_t *indices, const uint64_t count);
    explicit IndexBuffer(const std::vector<uint32_t> &indices);

    void Bind() const;
    void Unbind() const;

    constexpr uint64_t GetCount() const
    {
        return m_Count;
    }

private:
    const uint64_t m_Count;
};

#endif // INDEXBUFFER_HPP_

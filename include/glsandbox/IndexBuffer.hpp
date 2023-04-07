#ifndef INDEXBUFFER_HPP_
#define INDEXBUFFER_HPP_

#include <cstddef>
#include <cstdint>

#include "glsandbox/GLObject.hpp"

class IndexBuffer : public GLObject
{
public:
    IndexBuffer(const uint32_t *indices, const std::size_t count);

    void Bind() const;
    void Unbind() const;

    constexpr std::size_t GetCount() const;

private:
    const std::size_t m_Count;
};

#endif // INDEXBUFFER_HPP_

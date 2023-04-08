#ifndef GLOBJECT_HPP_
#define GLOBJECT_HPP_

#include <cstdint>

class GLObject
{
public:
    constexpr uint32_t GetId() const;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

protected:
    uint32_t m_Id;
};

#endif // GLOBJECT_HPP_

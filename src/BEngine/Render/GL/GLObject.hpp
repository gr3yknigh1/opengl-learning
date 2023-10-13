#ifndef BENGINE_RENDER_GL_GLOBJECT_HPP
#define BENGINE_RENDER_GL_GLOBJECT_HPP

#include <cstdint>

class GLObject
{
public:
    constexpr uint32_t GetId() const
    {
        return m_Id;
    }

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

protected:
    uint32_t m_Id;
};

#endif // BENGINE_RENDER_GL_GLOBJECT_HPP

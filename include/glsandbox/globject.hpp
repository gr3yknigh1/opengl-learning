#ifndef GLOBJECT_HPP_H
#define GLOBJECT_HPP_H

namespace glsandbox
{

class GLObject
{
public:
    unsigned int GetId() const
    {
        return m_Id;
    }

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

protected:
    unsigned int m_Id;
};

} // namespace glsandbox

#endif // GLOBJECT_HPP_H

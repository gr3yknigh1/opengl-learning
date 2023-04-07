#ifndef GLOBJECT_HPP_
#define GLOBJECT_HPP_

class GLObject
{
public:
    constexpr unsigned int GetId() const
    {
        return m_Id;
    }

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

protected:
    unsigned int m_Id;
};

#endif // GLOBJECT_HPP_

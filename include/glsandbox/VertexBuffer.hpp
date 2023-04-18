#ifndef VERTEXBUFFER_HPP_
#define VERTEXBUFFER_HPP_

#include <vector>

#include <GL/glew.h>

#include "glsandbox/GLObject.hpp"
#include "glsandbox/GLUtils.hpp"

class VertexBuffer : public GLObject
{
public:
    VertexBuffer(const void *data, const std::size_t size);

    template <typename T> VertexBuffer(const std::vector<T> &data)
    {
        std::size_t dataSize = data.size() * sizeof(T);

        GL_Call(glGenBuffers(1, &m_Id));
        Bind();
        GL_Call(glBufferData(GL_ARRAY_BUFFER, static_cast<long>(dataSize),
                             data.data(), GL_STATIC_DRAW));
    }

    void Bind() const;
    void Unbind() const;
};

#endif // VERTEXBUFFER_HPP_

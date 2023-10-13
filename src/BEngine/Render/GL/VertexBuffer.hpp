#ifndef BENGINE_RENDER_GL_VERTEXBUFFER_HPP
#define BENGINE_RENDER_GL_VERTEXBUFFER_HPP

#include <vector>

#include <GL/glew.h>

#include "BEngine/Render/GL/GLObject.hpp"
#include "BEngine/Render/GL/Utils.hpp"

class VertexBuffer : public GLObject
{
public:
    VertexBuffer(const void *data, const std::size_t size);

    template <typename T> VertexBuffer(const std::vector<T> &data)
    {
        std::size_t dataSize = data.size() * sizeof(T);

        GL_Call(glGenBuffers(1, &m_Id));
        Bind();
        GL_Call(glBufferData(GL_ARRAY_BUFFER, static_cast<long>(dataSize), data.data(), GL_STATIC_DRAW));
    }

    void Bind() const;
    void Unbind() const;
};

#endif // BENGINE_RENDER_GL_VERTEXBUFFER_HPP

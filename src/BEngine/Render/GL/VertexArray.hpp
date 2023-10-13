#ifndef BENGINE_RENDER_GL_VERTEXARRAY_HPP
#define BENGINE_RENDER_GL_VERTEXARRAY_HPP

#include <GL/glew.h>

#include "BEngine/Render/GL/GLObject.hpp"
#include "BEngine/Render/GL/VertexBuffer.hpp"
#include "BEngine/Render/GL/VertexBufferLayout.hpp"

class VertexArray : public GLObject
{
public:
    VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
};

#endif // BENGINE_RENDER_GL_VERTEXARRAY_HPP

#ifndef VERTEXARRAY_HPP_
#define VERTEXARRAY_HPP_

#include <GL/glew.h>

#include "glsandbox/GLObject.hpp"
#include "glsandbox/VertexBuffer.hpp"
#include "glsandbox/VertexBufferLayout.hpp"

class VertexArray : public GLObject
{
public:
    VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
};

#endif // VERTEXARRAY_HPP_

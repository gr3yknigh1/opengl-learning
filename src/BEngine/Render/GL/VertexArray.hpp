#ifndef VERTEXARRAY_HPP_
#define VERTEXARRAY_HPP_

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

#endif // VERTEXARRAY_HPP_

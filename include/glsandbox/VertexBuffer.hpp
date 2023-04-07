#ifndef VERTEXBUFFER_HPP_
#define VERTEXBUFFER_HPP_

#include <GL/glew.h>

#include "glsandbox/GLObject.hpp"
#include "glsandbox/GLUtils.hpp"

class VertexBuffer : public GLObject
{
public:
    VertexBuffer(const void *data, const std::size_t size);

    void Bind() const;
    void Unbind() const;
};

#endif // VERTEXBUFFER_HPP_

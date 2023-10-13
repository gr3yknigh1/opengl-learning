#include "BEngine/Render/Renderer.hpp"

#include <GL/glew.h>

#include "BEngine/Render/GL/Utils.hpp"

using namespace BE::Render;

void Renderer::BeginDraw(const Camera3D &camera)
{
}

void Renderer::SetClearColor(const glm::vec4 &clearColor)
{
    GL_Call(glClearColor(clearColor.r, clearColor.b, clearColor.g, clearColor.a));
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    GL_Call(glClearColor(r, b, g, a));
}

void Renderer::Clear(void)
{
    GL_Call(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::EndDraw(void)
{
}

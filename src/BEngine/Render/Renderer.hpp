#ifndef BENGINE_RENDER_RENDERER_HPP
#define BENGINE_RENDER_RENDERER_HPP

#include <glm/glm.hpp>

#include "BEngine/Render/Camera3D.hpp"

namespace BE::Render
{

class Renderer
{
public:
    static void BeginDraw(const Camera3D &camera);

    static void SetClearColor(const glm::vec4 &clearColor);
    static void SetClearColor(float r, float g, float b, float a);

    static void Clear(void);

    static void EndDraw(void);
};

} // namespace BE::Render

#endif // BENGINE_RENDER_RENDERER_HPP

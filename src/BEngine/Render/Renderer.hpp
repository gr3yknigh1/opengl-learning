#ifndef RENDERER_HPP_
#define RENDERER_HPP_

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

#endif // RENDERER_HPP_

#ifndef BENGINE_RENDER_GL_MATERIAL_HPP
#define BENGINE_RENDER_GL_MATERIAL_HPP

#include "BEngine/Core/Memory.hpp"
#include "BEngine/Render/GL/Texture.hpp"

struct Material
{
    Ref<Texture> diffuse;
    Ref<Texture> specular;
    Ref<Texture> emission;
};

#endif // BENGINE_RENDER_GL_MATERIAL_HPP

#ifndef BENGINE_RENDER_GL_MATERIAL_HPP
#define BENGINE_RENDER_GL_MATERIAL_HPP

#include "BEngine/Core/Memory.hpp"
#include "BEngine/Render/GL/Texture.hpp"

struct Material
{
    BE::Core::Memory::Ref<Texture> diffuse;
    BE::Core::Memory::Ref<Texture> specular;
    BE::Core::Memory::Ref<Texture> emission;
};

#endif // BENGINE_RENDER_GL_MATERIAL_HPP

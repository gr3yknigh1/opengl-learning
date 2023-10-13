#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include "BEngine/Core/Memory.hpp"
#include "BEngine/Render/GL/Texture.hpp"

struct Material
{
    Ref<Texture> diffuse;
    Ref<Texture> specular;
    Ref<Texture> emission;
};

#endif // MATERIAL_HPP_

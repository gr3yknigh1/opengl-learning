#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include "glsandbox/Texture.hpp"
#include "glsandbox/Core/Refs.hpp"

struct Material {
    Ref<Texture> diffuse;
    Ref<Texture> specular;
    Ref<Texture> emission;
};

#endif // MATERIAL_HPP_

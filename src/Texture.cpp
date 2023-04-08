#include <stdexcept>

#include <fmt/format.h>
#include <stb_image.h>

#include "glsandbox/GLUtils.hpp"
#include "glsandbox/Texture.hpp"

// NOTE: Currently channel parameter doens't not affect
Texture::Texture(const std::filesystem::path &texturePath)
    : m_TexturePath(texturePath), m_Buffer(nullptr), m_Width(0), m_Height(0),
      m_BPP(0)
{
    if (!std::filesystem::exists(texturePath))
    {
        throw std::runtime_error(
            fmt::format("Path doesn't exists '{}'", texturePath.c_str()));
    }

    stbi_set_flip_vertically_on_load(true);
    // TODO: Add channels field
    m_Buffer = stbi_load(texturePath.c_str(), reinterpret_cast<int *>(&m_Width),
                         reinterpret_cast<int *>(&m_Height),
                         reinterpret_cast<int *>(&m_BPP), 4);

    if (m_Buffer == nullptr)
    {
        throw std::runtime_error(fmt::format("Error during image loading: '{}'",
                                             texturePath.c_str()));
    }

    GL_Call(glGenTextures(1, &m_Id));
    GL_Call(glBindTexture(GL_TEXTURE_2D, m_Id));

    GL_Call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer));

    GL_Call(glGenerateMipmap(GL_TEXTURE_2D));

    GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    // NOTE: _S and _T is like x and y for textures
    GL_Call(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
    GL_Call(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

    Unbind();

    stbi_image_free(m_Buffer);
}

Texture::~Texture()
{
    Delete();
}

void Texture::Delete() const
{
    GL_Call(glDeleteTextures(1, &m_Id));
}

void Texture::Bind() const
{
    GL_Call(glActiveTexture(GL_TEXTURE0));
    GL_Call(glBindTexture(GL_TEXTURE_2D, m_Id));
}

void Texture::Bind(const uint32_t slot) const
{
    GL_Call(glActiveTexture(GL_TEXTURE0 + slot));
    GL_Call(glBindTexture(GL_TEXTURE_2D, m_Id));
}

void Texture::Unbind() const
{
    GL_Call(glBindTexture(GL_TEXTURE_2D, 0));
}

constexpr int Texture::GetWidth() const
{
    return m_Width;
}

constexpr int Texture::GetHeight() const
{
    return m_Height;
}

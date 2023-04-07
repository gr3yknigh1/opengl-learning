#include <stb_image.h>

#include "glsandbox/GLUtils.hpp"
#include "glsandbox/Texture.hpp"

// NOTE: Currently channel parameter doens't not affect
Texture::Texture(const char *filePath)
    : m_FilePath(filePath), m_Buffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{

    stbi_set_flip_vertically_on_load(1);

    m_Buffer = stbi_load(filePath, &m_Width, &m_Height, &m_BPP, 4);

    GL_Call(glGenTextures(1, &m_Id));
    GL_Call(glBindTexture(GL_TEXTURE_2D, m_Id));

    GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // NOTE: _S and _T is like x and y for textures
    GL_Call(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_Call(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL_Call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer));

    GL_Call(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_Buffer)
    {
        stbi_image_free(m_Buffer);
    }
}

Texture::~Texture()
{
    Delete();
}

inline void Texture::Delete() const
{
    GL_Call(glDeleteTextures(1, &m_Id));
}

inline void Texture::Bind() const
{
    GL_Call(glActiveTexture(GL_TEXTURE0));
    GL_Call(glBindTexture(GL_TEXTURE_2D, m_Id));
}

void Texture::Bind(uint32_t slot) const
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

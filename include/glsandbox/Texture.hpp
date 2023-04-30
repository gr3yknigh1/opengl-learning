#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <cstdint>
#include <filesystem>
#include <string>

#include "glsandbox/GLObject.hpp"

enum TextureType
{
    None,
    Diffuse,
    Specular,
    Emission,
};

const char *TextureTypeToString(TextureType type);

class Texture : public GLObject
{
public:
    Texture(const std::filesystem::path &texturePath);
    Texture(const std::filesystem::path &texturePath, const TextureType type);
    // Texture(const Texture &texture);
    ~Texture();

    void Delete() const;

    void Bind() const;
    void BindTo(const uint32_t slot) const;
    static void Activate(const uint32_t slot);
    void Unbind() const;

    void SetOptionMinFilter(int option) const;
    void SetOptionMagFilter(int option) const;
    void SetOptionWrapsS(int option) const;
    void SetOptionWrapsT(int option) const;

    constexpr int GetWidth() const
    {
        return m_Width;
    }

    constexpr int GetHeight() const
    {
        return m_Height;
    }

    constexpr TextureType GetType() const
    {
        return m_Type;
    }

    Texture &operator=(const Texture& texture);

private:
    std::string m_TexturePath;
    unsigned char *m_Buffer;
    TextureType m_Type;

    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_BPP; // Bits Per Pixel

    void Initialize(void);
};

#endif // TEXTURE_HPP_

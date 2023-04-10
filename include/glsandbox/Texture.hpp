#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <cstdint>
#include <filesystem>
#include <string>

#include "glsandbox/GLObject.hpp"

class Texture : GLObject
{
public:
    Texture(const std::filesystem::path &imagePath);
    ~Texture();

    void Delete() const;

    void Bind() const;
    void BindTo(const uint32_t slot) const;
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

private:
    const std::string m_TexturePath;
    unsigned char *m_Buffer;

    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_BPP; // Bits Per Pixel
};

#endif // TEXTURE_HPP_

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
    void Bind(const uint32_t slot) const;
    void Unbind() const;

    constexpr int GetWidth() const;
    constexpr int GetHeight() const;

private:
    const std::string m_TexturePath;
    unsigned char *m_Buffer;

    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_BPP; // Bits Per Pixel
};

#endif // TEXTURE_HPP_

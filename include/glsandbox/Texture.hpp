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

    inline void Delete() const;

    void Bind() const;
    void Bind(uint32_t slot) const;
    void Unbind() const;

    constexpr int GetWidth() const;
    constexpr int GetHeight() const;

private:
    std::string m_FilePath;
    unsigned char *m_Buffer;

    int m_Width;
    int m_Height;
    int m_BPP; // Bits Per Pixel
};

#endif // TEXTURE_HPP_

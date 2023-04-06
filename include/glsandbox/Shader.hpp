#ifndef SHADER_HPP_
#define SHADER_HPP_

#include "glsandbox/globject.hpp"
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

namespace glsandbox
{

// TODO: Separate shader compilation login in `ShaderTranslationUnit` and leave
// here only shader program logic
class Shader : public GLObject
{
public:
    ~Shader();

    void Bind() const;
    void Unbind() const;

    const std::optional<uint32_t> TryGetUniformLocation(
        const std::string &name) const;

    template <typename UniformType>
    void SetUniform(const std::string &name, UniformType value) const;

    // TODO: Move to sep FS util class
    static std::string ReadFile(const std::filesystem::path &filePath);

    static Shader FromSourceFiles(const std::filesystem::path &vertexSource,
                                  const std::filesystem::path &fragmentSource);

    // TODO: Replace `shaderType` with enumerator
    static uint32_t CompileShader(const std::filesystem::path &shaderFilePath,
                                  const std::string &shaderSource,
                                  int shaderType);

    static uint32_t LinkShaderProgram(uint32_t vertexShaderId,
                                      uint32_t fragmentShaderId);

private:
    Shader();
};

} // namespace glsandbox

#endif // SHADER_HPP_

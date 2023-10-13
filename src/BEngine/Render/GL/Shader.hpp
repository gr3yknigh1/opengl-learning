#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

#include <glm/glm.hpp>

#include "BEngine/Render/GL/GLObject.hpp"

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

    void SetUniform(const std::string &name, bool value) const;
    void SetUniform(const std::string &name, int32_t value) const;
    void SetUniform(const std::string &name, float value) const;
    void SetUniform(const std::string &name, const glm::vec3 &value) const;
    void SetUniform(const std::string &name, const glm::mat4 &value) const;

    // TODO: Move to sep FS util class
    static std::string ReadFile(const std::filesystem::path &filePath);

    static Shader FromSourceFiles(const std::filesystem::path &vertexSource,
                                  const std::filesystem::path &fragmentSource);

    // TODO: Replace `shaderType` with enumerator
    static uint32_t CompileShader(const std::filesystem::path &shaderFilePath,
                                  int shaderType);

    static uint32_t LinkShaderProgram(uint32_t vertexShaderId,
                                      uint32_t fragmentShaderId);

private:
    Shader();
};

#endif // SHADER_HPP_

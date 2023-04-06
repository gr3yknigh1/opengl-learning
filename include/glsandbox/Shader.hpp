#ifndef SHADER_HPP_
#define SHADER_HPP_

#include "glsandbox/globject.hpp"
#include <filesystem>
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

    // TODO: Move to sep FS util class
    static std::string ReadFile(const std::filesystem::path &filePath);

    static Shader FromSourceFiles(const std::filesystem::path &vertexSource,
                                  const std::filesystem::path &fragmentSource);

    // TODO: Replace `shaderType` with enumerator
    static unsigned int CompileShader(
        const std::filesystem::path &shaderFilePath,
        const std::string &shaderSource, int shaderType);

    static unsigned int LinkShaderProgram(unsigned int vertexShaderId,
                                          unsigned int fragmentShaderId);

private:
    Shader();
};

} // namespace glsandbox

#endif // SHADER_HPP_

#ifndef SHADER_HPP_
#define SHADER_HPP_

#include "glsandbox/globject.hpp"
#include "glsandbox/glutils.hpp"
#include <GL/glew.h>
#include <filesystem>
#include <fstream>
#include <string>

namespace glsandbox
{

// TODO: Separate shader compilation login in `ShaderTranslationUnit` and leave
// here only shader program logic
class Shader : public GLObject
{
public:
    ~Shader()
    {
        GL_Call(glDeleteProgram(m_Id));
    }

    void Bind() const
    {
        GL_Call(glUseProgram(m_Id));
    }

    void Unbind() const
    {
    }

    // TODO: Move to sep FS util class
    static std::string ReadFile(std::filesystem::path filePath)
    {
        if (!std::filesystem::exists(filePath))
        {
            std::printf("[APP]: File '%s' doesn't exists", filePath.c_str());
        }

        std::ifstream fstream(filePath, std::ios::in | std::ios::binary);
        if (!fstream.is_open())
        {
            std::printf("[APP]: Can't open file '%s'", filePath.c_str());
            std::exit(EXIT_FAILURE);
            // NOTE: Exit somehow?
        }

        const auto fileSize = std::filesystem::file_size(filePath);
        std::string fileContent(fileSize, '\0');
        fstream.read(fileContent.data(), fileSize);
        return fileContent;
    }

    static Shader FromSourceFiles(std::filesystem::path vertexSource,
                            std::filesystem::path fragmentSource)
    {
        unsigned int vertexShaderId = CompileShader(
            vertexSource, ReadFile(vertexSource), GL_VERTEX_SHADER);
        unsigned int fragmentShaderId = CompileShader(
            fragmentSource, ReadFile(fragmentSource), GL_FRAGMENT_SHADER);
        unsigned int shaderProgramId =
            LinkShaderProgram(vertexShaderId, fragmentShaderId);

        GL_Call(glDeleteShader(vertexShaderId));
        GL_Call(glDeleteShader(fragmentShaderId));

        Shader shader;
        shader.m_Id = shaderProgramId;
        return shader;
    }

    // TODO: Replace `shaderType` with enumerator
    static unsigned int CompileShader(std::filesystem::path shaderFilePath,
                                      const std::string &shaderSource,
                                      int shaderType)
    {
        int success;
        char shaderCompilationInfoLog[GL_INFO_LOG_LENGTH];
        const char *shaderSourceData = shaderSource.c_str();

        unsigned int shaderId = 0;
        GL_CallO(glCreateShader(shaderType), &shaderId);
        GL_Call(glShaderSource(shaderId, 1, &shaderSourceData, nullptr));
        GL_Call(glCompileShader(shaderId));
        GL_Call(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success));

        if (!success)
        {
            GL_Call(glGetShaderInfoLog(shaderId, GL_INFO_LOG_LENGTH, nullptr,
                                       shaderCompilationInfoLog));
            std::printf("[GL]: Error during shader compilation: '%s' : %s\n",
                        shaderFilePath.c_str(), shaderCompilationInfoLog);
            std::exit(EXIT_FAILURE);
            return 0;
        }

        return shaderId;
    }

    static unsigned int LinkShaderProgram(unsigned int vertexShaderId,
                                          unsigned int fragmentShaderId)

    {
        int success;
        char shaderCompilationInfoLog[GL_INFO_LOG_LENGTH];

        unsigned int shaderProgramId = 0;
        GL_CallO(glCreateProgram(), &shaderProgramId);

        GL_Call(glAttachShader(shaderProgramId, vertexShaderId));
        GL_Call(glAttachShader(shaderProgramId, fragmentShaderId));
        GL_Call(glLinkProgram(shaderProgramId));

        GL_Call(glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success));

        if (!success)
        {
            GL_Call(glGetProgramInfoLog(shaderProgramId, GL_INFO_LOG_LENGTH,
                                        nullptr, shaderCompilationInfoLog));
            std::exit(EXIT_FAILURE);
        }

        return shaderProgramId;
    }

private:
    Shader(){};
};

} // namespace glsandbox

#endif // SHADER_HPP_

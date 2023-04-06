#include <cstdio>
#include <fstream>
#include <optional>
#include <stdexcept>

#include <GL/glew.h>
#include <fmt/format.h>

#include "glsandbox/Shader.hpp"
#include "glsandbox/glutils.hpp"

namespace glsandbox
{

Shader::~Shader()
{
    GL_Call(glDeleteProgram(m_Id));
}

void Shader::Bind() const
{
    GL_Call(glUseProgram(m_Id));
}

void Shader::Unbind() const
{
}

const std::optional<uint32_t> Shader::TryGetUniformLocation(
    const std::string &name) const
{
    Bind();

    int32_t uniformLocation = -1;
    GL_CallO(glGetUniformLocation(GetId(), name.c_str()), &uniformLocation);
    return uniformLocation >= 0
               ? std::make_optional(static_cast<uint32_t>(uniformLocation))
               : std::nullopt;
}

template <> void Shader::SetUniform(const std::string &name, bool value) const
{
    Bind();

    const auto uniformLocation = TryGetUniformLocation(name);

    if (!uniformLocation.has_value())
    {
        throw std::invalid_argument(
            fmt::format("Error: Can't find location of uniform '{}'", name));
    }

    GL_Call(glUniform1i(uniformLocation.value(), static_cast<int32_t>(value)));
}

template <> void Shader::SetUniform(const std::string &name, float value) const
{
    Bind();

    const auto uniformLocation = TryGetUniformLocation(name);

    if (!uniformLocation.has_value())
    {
        throw std::invalid_argument(
            fmt::format("Error: Can't find location of uniform '{}'", name));
    }

    GL_Call(glUniform1f(uniformLocation.value(), value));
}

std::string Shader::ReadFile(const std::filesystem::path &filePath)
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

Shader Shader::FromSourceFiles(const std::filesystem::path &vertexSource,
                               const std::filesystem::path &fragmentSource)
{
    uint32_t vertexShaderId =
        CompileShader(vertexSource, ReadFile(vertexSource), GL_VERTEX_SHADER);
    uint32_t fragmentShaderId = CompileShader(
        fragmentSource, ReadFile(fragmentSource), GL_FRAGMENT_SHADER);
    uint32_t shaderProgramId =
        LinkShaderProgram(vertexShaderId, fragmentShaderId);

    GL_Call(glDeleteShader(vertexShaderId));
    GL_Call(glDeleteShader(fragmentShaderId));

    Shader shader;
    shader.m_Id = shaderProgramId;
    return shader;
}

uint32_t Shader::CompileShader(const std::filesystem::path &shaderFilePath,
                               const std::string &shaderSource, int shaderType)
{
    int success;
    char shaderCompilationInfoLog[GL_INFO_LOG_LENGTH];
    const char *shaderSourceData = shaderSource.c_str();

    uint32_t shaderId = 0;
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

uint32_t Shader::LinkShaderProgram(uint32_t vertexShaderId,
                                   uint32_t fragmentShaderId)

{
    int success;
    char shaderCompilationInfoLog[GL_INFO_LOG_LENGTH];

    uint32_t shaderProgramId = 0;
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

Shader::Shader(){};

} // namespace glsandbox

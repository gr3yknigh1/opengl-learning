#include <cstdio>
#include <fstream>
#include <optional>
#include <stdexcept>

#include <GL/glew.h>
#include <fmt/format.h>
#include <glm/glm.hpp>

#include "glsandbox/GLUtils.hpp"
#include "glsandbox/Shader.hpp"

static const std::size_t GL_SHADER_COMPILATION_LOG_BUFFER_SIZE = 512;

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

void Shader::SetUniform(const std::string &name, bool value) const
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

void Shader::SetUniform(const std::string &name, int32_t value) const
{
    Bind();

    const auto uniformLocation = TryGetUniformLocation(name);

    if (!uniformLocation.has_value())
    {
        throw std::invalid_argument(
            fmt::format("Error: Can't find location of uniform '{}'", name));
    }

    GL_Call(glUniform1i(uniformLocation.value(), value));
}

void Shader::SetUniform(const std::string &name, float value) const
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

void Shader::SetUniform(const std::string &name, const glm::vec3 &value) const
{
    Bind();

    const auto uniformLocation = TryGetUniformLocation(name);

    if (!uniformLocation.has_value())
    {
        throw std::invalid_argument(
            fmt::format("Error: Can't find location of uniform '{}'", name));
    }

    GL_Call(glUniform3f(uniformLocation.value(), value.x, value.y, value.z));
}

std::string Shader::ReadFile(const std::filesystem::path &filePath)
{
    if (!std::filesystem::exists(filePath))
    {
        std::invalid_argument(
            fmt::format("[APP]: File '{}' doesn't exists", filePath.c_str()));
    }

    std::ifstream fstream(filePath, std::ios::in | std::ios::binary);
    if (!fstream.is_open())
    {
        std::invalid_argument(
            fmt::format("[APP]: Can't open file '%s'", filePath.c_str()));
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
    char shaderCompilationInfoLog[GL_SHADER_COMPILATION_LOG_BUFFER_SIZE];
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
        throw std::runtime_error(
            fmt::format("[GL]: Error during shader compilation: '{}' : {}",
                        shaderFilePath.c_str(), shaderCompilationInfoLog)

        );
    }

    return shaderId;
}

uint32_t Shader::LinkShaderProgram(uint32_t vertexShaderId,
                                   uint32_t fragmentShaderId)

{
    int success;
    char shaderCompilationInfoLog[GL_SHADER_COMPILATION_LOG_BUFFER_SIZE];

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
        throw std::runtime_error(
            fmt::format("[GL]: Error during shader program linking: {}",
                        shaderCompilationInfoLog)

        );
    }

    return shaderProgramId;
}

Shader::Shader(){};

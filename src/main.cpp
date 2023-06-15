/*
 * OpenGL Sandbox
 * Basic Triangle
 */

#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "glsandbox/glutils.hpp"

static int32_t CompileShader(const char *shaderPath, int shaderType)
{
    int32_t success;
    char shaderCompilationInfoLog[GL_INFO_LOG_LENGTH];

    std::ifstream fstream(shaderPath, std::ios::in | std::ios::binary);
    if (!fstream.is_open())
    {
        std::printf("Error: Can't open file '%s'\n", shaderPath);
        std::exit(EXIT_FAILURE);
    }
    const uintmax_t fileSize = std::filesystem::file_size(shaderPath);
    std::string shaderSource(fileSize, '\0');
    fstream.read(shaderSource.data(), fileSize);

    const char *const shaderData = shaderSource.data();

    uint32_t shaderId = 0;
    GL_CallO(glCreateShader(shaderType), &shaderId);
    GL_Call(glShaderSource(shaderId, 1, &shaderData, nullptr));
    GL_Call(glCompileShader(shaderId));
    GL_Call(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success));

    if (!success)
    {
        GL_Call(glGetShaderInfoLog(shaderId, GL_INFO_LOG_LENGTH, nullptr,
                                   shaderCompilationInfoLog));
        std::printf("[GL]: Error during shader compilation: '%s' : %s\n",
                    shaderPath, shaderCompilationInfoLog);
        std::exit(EXIT_FAILURE);
    }

    return shaderId;
}

static uint32_t LinkShaderProgram(uint32_t vertexShaderId,
                                  uint32_t fragmentShaderId)

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

int main(void)
{
    // GLFW initialization
    if (!glfwInit())
    {
        std::cerr << "[GLFW]: Error! Failed initialize glfw\n";
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window creation
    uint32_t windowWidth = 900;
    uint32_t windowHeight = 600;
    const char *windowTitle = "OpenGL Triangle";
    GLFWwindow *window = glfwCreateWindow(static_cast<int>(windowWidth),
                                          static_cast<int>(windowHeight),
                                          windowTitle, nullptr, nullptr);

    // Check if window created successfuly
    if (!window)
    {
        glfwTerminate();
        std::cerr << "[GLFW]: Error! Failed create window\n";
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(true);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    // Glew Initialization
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        std::cerr << "[GLEW]: Error! Failed initialize glew\n";
        return EXIT_FAILURE;
    }

    GL_Call(glViewport(0, 0, static_cast<int>(windowWidth),
                       static_cast<int>(windowHeight)));
    GL_Call(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    float vertices[] = {
        // positions        // colors         // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };
    uint32_t indices[] = {0, 1, 2, 0, 2, 3};

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    const char *texturePath = "assets/textures/texture.jpeg";
    unsigned char *data =
        stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data == nullptr)
    {
        std::cerr << "Error during loading of '" << texturePath << "'\n";
        return EXIT_FAILURE;
    }

    uint32_t textureId;
    GL_Call(glGenTextures(1, &textureId));
    GL_Call(glBindTexture(GL_TEXTURE_2D, textureId));
    GL_Call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, data));
    GL_Call(glGenerateMipmap(GL_TEXTURE_2D));

    GL_Call(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
    GL_Call(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

    uint32_t vbo = 0, vao = 0, ebo = 0;

    // NOTE: it must be before `glVertexAttribPointer` call
    GL_Call(glGenVertexArrays(1, &vao));
    GL_Call(glBindVertexArray(vao));

    GL_Call(glGenBuffers(1, &vbo));
    GL_Call(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_Call(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                         GL_STATIC_DRAW));

    GL_Call(glGenBuffers(1, &ebo));
    GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL_Call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                         GL_STATIC_DRAW));

    GL_Call(
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0));
    GL_Call(glEnableVertexAttribArray(0));
    // color attribute
    GL_Call(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                  (void *)(3 * sizeof(float))));
    GL_Call(glEnableVertexAttribArray(1));

    GL_Call(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                  (void *)(6 * sizeof(float))));
    GL_Call(glEnableVertexAttribArray(2));

    uint32_t vertexShaderId =
        CompileShader("./assets/shaders/sample.vs", GL_VERTEX_SHADER);
    uint32_t fragmentShaderId =
        CompileShader("./assets/shaders/sample.fs", GL_FRAGMENT_SHADER);
    uint32_t shaderProgramId =
        LinkShaderProgram(vertexShaderId, fragmentShaderId);
    GL_Call(glDeleteShader(vertexShaderId));
    GL_Call(glDeleteShader(fragmentShaderId));
    GL_Call(glUseProgram(shaderProgramId));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    int uniformPos = glGetUniformLocation(shaderProgramId, "Texture");
    glUniform1i(uniformPos, GL_TEXTURE0);

    while (!glfwWindowShouldClose(window))
    {
        GL_Call(glClear(GL_COLOR_BUFFER_BIT));
        GL_Call(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GL_Call(glBindVertexArray(vao));
        GL_Call(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

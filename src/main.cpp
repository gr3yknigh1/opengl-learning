#include <cmath>
#include <cstdint>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "fmt/core.h"
#include "glsandbox/Shader.hpp"
#include "glsandbox/glutils.hpp"

const char *GLFW_ErrorCodeDispatch(int errorCode)
{
    switch (errorCode)
    {
    case GLFW_NOT_INITIALIZED:
        return STRINGIFY(GLFW_NOT_INITIALIZED);
    case GLFW_NO_CURRENT_CONTEXT:
        return STRINGIFY(GLFW_NO_CURRENT_CONTEXT);
    case GLFW_INVALID_ENUM:
        return STRINGIFY(GLFW_INVALID_ENUM);
    case GLFW_OUT_OF_MEMORY:
        return STRINGIFY(GLFW_OUT_OF_MEMORY);
    case GLFW_API_UNAVAILABLE:
        return STRINGIFY(GLFW_API_UNAVAILABLE);
    case GLFW_VERSION_UNAVAILABLE:
        return STRINGIFY(GLFW_VERSION_UNAVAILABLE);
    case GLFW_PLATFORM_ERROR:
        return STRINGIFY(GLFW_PLATFORM_ERROR);
    case GLFW_FORMAT_UNAVAILABLE:
        return STRINGIFY(GLFW_FORMAT_UNAVAILABLE);
    case GLFW_NO_WINDOW_CONTEXT:
        return STRINGIFY(GLFW_NO_WINDOW_CONTEXT);
    default:
        return "GLFW_UNHANDLED_ERROR";
    }

    return nullptr;
}

void GLFW_FrameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    GL_Call(glViewport(0, 0, width, height));
}

void GLFW_ErrorHandler(int errorCode, const char *description)
{
    std::printf("[GLFW]: %s - error code %d, description: '%s'\n",
                GLFW_ErrorCodeDispatch(errorCode), errorCode, description);
}

void GLFW_KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                      int mods)
{
    if ((key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS)
    {
        GL_Call(glfwSetWindowShouldClose(window, true));
    }
}

void PrintDebugInfo()
{
    const unsigned char *glVersion = nullptr;
    GL_CallO(glGetString(GL_VERSION), &glVersion);

    const unsigned char *glGlslVersion = nullptr;
    GL_CallO(glGetString(GL_SHADING_LANGUAGE_VERSION), &glGlslVersion);

    const unsigned char *glRendererVersion = nullptr;
    GL_CallO(glGetString(GL_RENDERER), &glRendererVersion);

    const unsigned char *glVendor = nullptr;
    GL_CallO(glGetString(GL_VENDOR), &glVendor);

    std::printf("[GLFW]: %s\n", glfwGetVersionString());
    std::printf("[GL]: OpenGL  : %s, GLSL: %s\n", glVersion, glGlslVersion);
    std::printf("[GL]: Renderer: %s\n", glRendererVersion);
    std::printf("[GL]: Vendor  : %s\n", glVendor);
}

int main(void)
{
    using namespace glsandbox;

    if (!glfwInit())
    {
        std::printf("[GLFW]: Error! Failed initialize glfw\n");
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // NOTE: For compatibility
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glm::vec2 windowSize = {900, 600};
    const char *title = "Triangle";
    GLFWwindow *window =
        glfwCreateWindow(windowSize.x, windowSize.y, title, nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        std::printf("[GLFW]: Error! Failed create window\n");
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(GLFW_ErrorHandler);
    glfwSetKeyCallback(window, GLFW_KeyCallback);
    glfwSetFramebufferSizeCallback(window, GLFW_FrameBufferSizeCallback);
    glfwSwapInterval(true);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        std::printf("[GLEW]: Error! Failed initialize glew\n");
        return EXIT_FAILURE;
    }

    PrintDebugInfo();

    GL_Call(glViewport(0, 0, windowSize.x, windowSize.y));
    GL_Call(glEnable(GL_BLEND));
    GL_Call(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // ------------------------------------------------------ //

    glm::vec4 clearColor = {.1f, .1f, .1f, 1.f};
    float vertices[] = {0.5,   -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
                        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f};

    unsigned int indices[] = {0, 1, 2};

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

    // position attribute
    GL_Call(
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0));
    GL_Call(glEnableVertexAttribArray(0));
    // color attribute
    GL_Call(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                                  (void *)(3 * sizeof(float))));
    GL_Call(glEnableVertexAttribArray(1));

    Shader shader =
        Shader::FromSourceFiles(ASSETS_DIR "/shaders/basic_vertex.glsl",
                                ASSETS_DIR "/shaders/basic_fragment.glsl");
    shader.SetUniform<float>("mod", 0.2f);

#if 1
    GL_Call(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
#else
    GL_Call(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
#endif

    while (!glfwWindowShouldClose(window))
    {
        GL_Call(glClearColor(clearColor.r, clearColor.b, clearColor.g,
                             clearColor.a));
        GL_Call(glClear(GL_COLOR_BUFFER_BIT));

        shader.Bind();
        GL_Call(glBindVertexArray(vao));
        GL_Call(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ------------------------------------------------------ //

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

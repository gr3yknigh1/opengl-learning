#include <cstdint>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define LOG(X) std::cout << X << '\n'

void GL_ClearErrors()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

void GL_CheckErrors(const char *glFunctionName, const char *sourceFilePath,
                    uint32_t sourceLine)
{
    while (GLenum errorCode = glGetError())
    {
        std::printf("[GL]: Error! Code %d\n", errorCode);
        std::printf("[GL]: Error occur at %s::%d during %s call\n",
                    sourceFilePath, sourceLine, glFunctionName);
        std::exit(EXIT_FAILURE);
    }
}

#define GL_Call(x)                                                             \
    GL_ClearErrors();                                                          \
    x;                                                                         \
    GL_CheckErrors(#x, __FILE__, __LINE__)

void GLFW_FrameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void GLFW_ErrorHandler(int errorCode, const char *description)
{
    std::printf("[GLFW]: Error! Code %d. Desc: %s\n", errorCode, description);
}

void GLFW_KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                      int mods)
{
    if ((key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(void)
{
    if (!glfwInit())
    {
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // NOTE: For compatibility
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if (!glewInit())
    {
        return EXIT_FAILURE;
    }

    std::printf("[GLFW]: %s\n", glfwGetVersionString());

    glm::vec2 windowSize = {900, 600};
    const char *title = "Triangle";
    GLFWwindow *window =
        glfwCreateWindow(windowSize.x, windowSize.y, title, 0, 0);

    if (!window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(GLFW_ErrorHandler);
    glfwSetKeyCallback(window, GLFW_KeyCallback);
    glfwSetFramebufferSizeCallback(window, GLFW_FrameBufferSizeCallback);

    GL_Call(glViewport(0, 0, windowSize.x, windowSize.y));
    // ------------------------------------------------------ //

    glm::vec4 clearColor = {.1f, .1f, .1f, 1.f};

    while (!glfwWindowShouldClose(window))
    {
        GL_Call(glClearColor(clearColor.r, clearColor.b, clearColor.g,
                             clearColor.a));
        GL_Call(glClear(GL_COLOR_BUFFER_BIT));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ------------------------------------------------------ //

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

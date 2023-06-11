/*
 * OpenGL Sandbox
 * Basic Triangle
 */

#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    glViewport(0, 0, static_cast<int>(windowWidth),
               static_cast<int>(windowHeight));
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define LOG(X) std::cout << X << '\n'

void GLFWErrorHandler(int errorCode, const char *description)
{
    std::printf("[GLFW]: Error! Code %d. Desc: %s\n", errorCode, description);
}

void GLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action,
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

    if (!glewInit())
    {
        return EXIT_FAILURE;
    }

    std::printf("[GLFW]: %s\n", glfwGetVersionString());

    GLFWwindow *window = glfwCreateWindow(900, 600, "Triangle", 0, 0);
    if (!window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(GLFWErrorHandler);
    glfwSetKeyCallback(window, GLFWKeyCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // NOTE: For compatibility
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

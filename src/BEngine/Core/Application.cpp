#include "BEngine/Core/Application.hpp"

#include "BEngine/Render/GL/Utils.hpp"

using namespace BE::Core;
using namespace BE::Core::Memory;

Ref<Application> Application::GetInstance(void)
{
    if (s_Instance != nullptr)
    {
        return s_Instance;
    }

    if (!glfwInit())
    {
        std::fprintf(stderr, "[GLFW]: Error! Failed initialize glfw\n");
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // NOTE: For compatibility
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // TODO: Move to params
    const char *title = "Triangle";
    GLFWwindow *window =
        glfwCreateWindow(Application::WindowSize.x, Application::WindowSize.y, title, nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        std::fprintf(stderr, "[GLFW]: Error! Failed create window\n");
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(ErrorHandler);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSwapInterval(1);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        std::fprintf(stderr, "[GLEW]: Error! Failed initialize glew\n");
        std::exit(EXIT_FAILURE);
    }

    GL_Call(glViewport(0, 0, Application::WindowSize.x, Application::WindowSize.y));
    GL_Call(glEnable(GL_BLEND));
    GL_Call(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_Call(glEnable(GL_DEPTH_TEST));
    GL_Call(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    const char *glslVersion = "#version 130";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    s_Instance = MakeRef<Application>(window);
    s_Instance->PrintDebugInfo();
    s_Instance->ToggleCursor();
    return s_Instance;
}

Application::Application(GLFWwindow *window) : m_Window(window)
{
}

Application::~Application(void)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Application::ToggleCursor(void)
{
    if (IsCursorEnabled)
    {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    IsCursorEnabled = !IsCursorEnabled;
}

void Application::PrintDebugInfo(void)
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

bool Application::ShouldClose(void) const
{
    return glfwWindowShouldClose(m_Window);
}

void Application::SwapBuffers(void) const
{
    glfwSwapBuffers(m_Window);
}

void Application::PollEvents(void) const
{
    glfwPollEvents();
}

void Application::FrameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    GL_Call(glViewport(0, 0, width, height));
}

void Application::ErrorHandler(int errorCode, const char *description)
{
    std::printf("[GLFW]: %s - error code %d, description: '%s'\n", ErrorCodeDispatch(errorCode), errorCode,
                description);
}

void Application::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    KeyboardEvent.Invoke(window, key, scancode, action, mods);

    if ((key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS)
    {
        GL_Call(glfwSetWindowShouldClose(window, true));
    }

    if ((key == GLFW_KEY_H || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS)
    {
        GetInstance()->ToggleCursor();
    }
}

const char *Application::ErrorCodeDispatch(int errorCode)
{
    switch (errorCode)
    {
    case GLFW_NOT_INITIALIZED:
        return BE_STRINGIFY(GLFW_NOT_INITIALIZED);
    case GLFW_NO_CURRENT_CONTEXT:
        return BE_STRINGIFY(GLFW_NO_CURRENT_CONTEXT);
    case GLFW_INVALID_ENUM:
        return BE_STRINGIFY(GLFW_INVALID_ENUM);
    case GLFW_OUT_OF_MEMORY:
        return BE_STRINGIFY(GLFW_OUT_OF_MEMORY);
    case GLFW_API_UNAVAILABLE:
        return BE_STRINGIFY(GLFW_API_UNAVAILABLE);
    case GLFW_VERSION_UNAVAILABLE:
        return BE_STRINGIFY(GLFW_VERSION_UNAVAILABLE);
    case GLFW_PLATFORM_ERROR:
        return BE_STRINGIFY(GLFW_PLATFORM_ERROR);
    case GLFW_FORMAT_UNAVAILABLE:
        return BE_STRINGIFY(GLFW_FORMAT_UNAVAILABLE);
    case GLFW_NO_WINDOW_CONTEXT:
        return BE_STRINGIFY(GLFW_NO_WINDOW_CONTEXT);
    default:
        return "GLFW_UNHANDLED_ERROR";
    }

    return nullptr;
}

void Application::MouseCallback(GLFWwindow *window, double xPosition, double yPosition)
{
    MouseEvent.Invoke(window, xPosition, yPosition);
}

void Application::ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    ScrollEvent.Invoke(window, xOffset, yOffset);
}

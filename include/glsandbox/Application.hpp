#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "glsandbox/Core/Refs.hpp"
#include "glsandbox/Events/Event.hpp"
#include "glsandbox/GLUtils.hpp"
#include "glsandbox/Window.hpp"
#include "glsandbox/defs.hpp"

class Application
{
public:
    static inline bool IsCursorEnabled = true;
    static inline const glm::vec2 WindowSize = {900, 600};

    static inline Event<GLFWwindow *, int, int, int, int> KeyboardEvent;
    static inline Event<GLFWwindow *, double, double> MouseEvent;
    static inline Event<GLFWwindow *, double, double> ScrollEvent;

    static Ref<Application> GetInstance(void)
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
        GLFWwindow *window = glfwCreateWindow(Application::WindowSize.x,
                                              Application::WindowSize.y, title,
                                              nullptr, nullptr);

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

        GL_Call(glViewport(0, 0, Application::WindowSize.x,
                           Application::WindowSize.y));
        GL_Call(glEnable(GL_BLEND));
        GL_Call(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GL_Call(glEnable(GL_DEPTH_TEST));

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

    Application(GLFWwindow *window) : m_Window(window)
    {
    }

    ~Application()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void ToggleCursor()
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

    void PrintDebugInfo(void)
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

    bool ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void SwapBuffers()
    {
        glfwSwapBuffers(m_Window);
    }

    void PollEvents()
    {
        glfwPollEvents();
    }

    // TODO: Make `const`
    constexpr GLFWwindow *GetWindow() const
    {
        return m_Window;
    }

private:
    static inline Ref<Application> s_Instance = nullptr;

    static void FrameBufferSizeCallback(GLFWwindow *window, int width,
                                        int height)
    {
        GL_Call(glViewport(0, 0, width, height));
    }

    static void ErrorHandler(int errorCode, const char *description)
    {
        std::printf("[GLFW]: %s - error code %d, description: '%s'\n",
                    ErrorCodeDispatch(errorCode), errorCode, description);
    }

    static void KeyCallback(GLFWwindow *window, int key, int scancode,
                            int action, int mods)
    {
        KeyboardEvent.Invoke(window, key, scancode, action, mods);

        if ((key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) &&
            action == GLFW_PRESS)
        {
            GL_Call(glfwSetWindowShouldClose(window, true));
        }

        if ((key == GLFW_KEY_H || key == GLFW_KEY_ESCAPE) &&
            action == GLFW_PRESS)
        {
            GetInstance()->ToggleCursor();
        }
    }

    static const char *ErrorCodeDispatch(int errorCode)
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

    static void MouseCallback(GLFWwindow *window, double xPosition,
                              double yPosition)
    {
        MouseEvent.Invoke(window, xPosition, yPosition);
    }

    static void ScrollCallback(GLFWwindow *window, double xOffset,
                               double yOffset)
    {
        ScrollEvent.Invoke(window, xOffset, yOffset);
    }

    GLFWwindow *m_Window;
};

#endif // APPLICATION_HPP_

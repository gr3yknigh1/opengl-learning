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

    static Ref<Application> GetInstance(void);

    Application(GLFWwindow *window);
    ~Application(void);

    void ToggleCursor(void);
    void PrintDebugInfo(void);
    bool ShouldClose(void) const;
    void SwapBuffers(void) const;
    void PollEvents(void) const;

    constexpr GLFWwindow *GetWindow(void) const
    {
        return m_Window;
    }

private:
    static inline Ref<Application> s_Instance = nullptr;

    static void FrameBufferSizeCallback(GLFWwindow *window, int width,
                                        int height);
    static void ErrorHandler(int errorCode, const char *description);
    static void KeyCallback(GLFWwindow *window, int key, int scancode,
                            int action, int mods);

    static const char *ErrorCodeDispatch(int errorCode);

    static void MouseCallback(GLFWwindow *window, double xPosition,
                              double yPosition);

    static void ScrollCallback(GLFWwindow *window, double xOffset,
                               double yOffset);

    GLFWwindow *m_Window;
};

#endif // APPLICATION_HPP_

#ifndef BENGINE_CORE_APPLICATION_HPP
#define BENGINE_CORE_APPLICATION_HPP

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "BEngine/BEngine.hpp"

#include "BEngine/Core/Event.hpp"
#include "BEngine/Core/Memory.hpp"

namespace BE::Core
{

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

    static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height);
    static void ErrorHandler(int errorCode, const char *description);
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    static const char *ErrorCodeDispatch(int errorCode);

    static void MouseCallback(GLFWwindow *window, double xPosition, double yPosition);

    static void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);

    GLFWwindow *m_Window;
};

} // namespace BE::Core

#endif // BENGINE_CORE_APPLICATION_HPP

#ifndef BADENGINE_CORE_FRAMETIMER_HPP_
#define BADENGINE_CORE_FRAMETIMER_HPP_

#include <GLFW/glfw3.h>

class FrameTimer
{
public:
    float Tick(void);

    constexpr float GetDeltaTime(void) const
    {
        return m_DeltaTime;
    }

private:
    float m_DeltaTime;
    float m_LastFrameTime;
};

#endif // FRAMETIMER_HPP_

#ifndef BENGINE_CORE_FRAMETIMER_HPP
#define BENGINE_CORE_FRAMETIMER_HPP

#include <GLFW/glfw3.h>

namespace BE::Core
{

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

} // namespace BE::Core

#endif // BENGINE_CORE_FRAMETIMER_HPP

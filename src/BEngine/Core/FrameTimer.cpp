#include "BEngine/Core/FrameTimer.hpp"

using namespace BE::Core;

float FrameTimer::Tick(void)
{
    float currentFrame = static_cast<float>(glfwGetTime());
    m_DeltaTime = currentFrame - m_LastFrameTime;
    m_LastFrameTime = currentFrame;
    return m_DeltaTime;
}

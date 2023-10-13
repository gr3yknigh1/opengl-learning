#include "BEngine/Render/Camera3D.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>

using namespace BE::Core;
using namespace BE::Render;

Camera3D::Camera3D(const Transform3D &transform, float fov, float sensitivity, bool isYInverse, float speed,
                   float speedModifier)
    : m_Transform(transform), m_Fov(fov), m_Sensitivity(sensitivity), m_Speed(speed), m_SpeedModifier(speedModifier),
      m_IsYInverse(isYInverse), m_Front(0, 0, -1), m_Up(0, 1, 0), m_LastXPosition(900.0f / 2),
      m_LastYPosition(600.0f / 2), m_FirstLastPositionAssignment(true)
{
}

void Camera3D::Rotate(float xPosition, float yPosition)
{
    if (Application::IsCursorEnabled)
    {
        m_FirstLastPositionAssignment = true;
        return;
    }

    // TODO: Replace with some kind of struct which holds value and previous
    // value
    if (m_FirstLastPositionAssignment)
    {
        m_LastXPosition = xPosition;
        m_LastYPosition = yPosition;
        m_FirstLastPositionAssignment = false;
    }

    float xOffset = xPosition - m_LastXPosition;
    float yOffset = yPosition - m_LastYPosition;
    m_LastXPosition = xPosition;
    m_LastYPosition = yPosition;

    xOffset *= m_Sensitivity;
    yOffset *= m_Sensitivity;

    m_Transform.Rotation.y += xOffset;
    m_Transform.Rotation.x += yOffset * (m_IsYInverse ? 1 : -1);

    if (m_Transform.Rotation.x > 89.0f)
        m_Transform.Rotation.x = 89.0f;
    if (m_Transform.Rotation.x < -89.0f)
        m_Transform.Rotation.x = -89.0f;

    glm::vec3 newFront;
    newFront.x = cos(glm::radians(m_Transform.Rotation.y)) * cos(glm::radians(m_Transform.Rotation.x));
    newFront.y = sin(glm::radians(m_Transform.Rotation.x));
    newFront.z = sin(glm::radians(m_Transform.Rotation.y)) * cos(glm::radians(m_Transform.Rotation.x));
    m_Front = glm::normalize(newFront);
}

void Camera3D::Zoom(float xOffset, float yOffset)
{
    (void)xOffset;

    if (Application::IsCursorEnabled)
    {
        return;
    }

    m_Fov -= (float)yOffset;
    if (m_Fov < 0.0f)
    {
        m_Fov = 0.0f;
    }
    if (m_Fov > 360.0f)
    {
        m_Fov = 360.0f;
    }
}

void Camera3D::Update(GLFWwindow *window, float deltaTime)
{
    if (Application::IsCursorEnabled)
    {
        return;
    }

    float currentSpeed = m_Speed;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        currentSpeed *= m_SpeedModifier;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_Transform.Position += currentSpeed * deltaTime * m_Front;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_Transform.Position -= currentSpeed * deltaTime * m_Front;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_Transform.Position -= glm::normalize(glm::cross(m_Front, m_Up)) * currentSpeed * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_Transform.Position += glm::normalize(glm::cross(m_Front, m_Up)) * currentSpeed * deltaTime;
    }
}

glm::mat4 Camera3D::GetViewMatrix(void) const
{
    return glm::lookAt(m_Transform.Position, m_Transform.Position + m_Front, m_Up);
}

glm::mat4 Camera3D::GetProjectionMatrix(void) const
{
    // TODO: Expose to paramteres zNear and zFar
    return glm::perspective(glm::radians(m_Fov), Application::WindowSize.x / Application::WindowSize.y, 0.1f, 100.0f);
}

void Camera3D::DrawImGUI(void)
{
    ImGui::SliderFloat("FOV", &m_Fov, 0.0f, 360.0f);
    ImGui::InputFloat3("Camera Position", glm::value_ptr(m_Transform.Position));
    ImGui::InputFloat3("Camera Rotation", glm::value_ptr(m_Transform.Rotation));
}

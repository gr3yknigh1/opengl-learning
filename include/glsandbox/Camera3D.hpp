#ifndef CAMERA3D_HPP_
#define CAMERA3D_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "glsandbox/Application.hpp"
#include "glsandbox/Transform3D.hpp"

class Camera3D
{
public:
    Camera3D() = delete;
    Camera3D(Camera3D &&other) = delete;

    Camera3D(const Transform3D &transform, float fov = 45,
             float sensitivity = 0.1, bool isYInverse = false,
             float speed = 2.5, float speedModifier = 2);

    constexpr Transform3D &GetTransform(void)
    {
        return m_Transform;
    }

    void Rotate(float xPosition, float yPosition);
    void Zoom(float xOffset, float yOffset);
    void Update(GLFWwindow *window, float deltaTime);

    glm::mat4 GetViewMatrix(void) const;
    glm::mat4 GetProjectionMatrix(void) const;

    void DrawImGUI(void);

private:
    Transform3D m_Transform;

    float m_Fov;
    float m_Sensitivity;
    float m_Speed;
    float m_SpeedModifier;
    bool m_IsYInverse;

    glm::vec3 m_Front;
    glm::vec3 m_Up;

    float m_LastXPosition;
    float m_LastYPosition;
    bool m_FirstLastPositionAssignment;
};

#endif // CAMERA3D_HPP_

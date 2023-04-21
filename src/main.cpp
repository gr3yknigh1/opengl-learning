#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "glm/ext/matrix_transform.hpp"
#include "glsandbox/Application.hpp"
#include "glsandbox/Camera3D.hpp"
#include "glsandbox/FrameTimer.hpp"
#include "glsandbox/GLUtils.hpp"
#include "glsandbox/IndexBuffer.hpp"
#include "glsandbox/Renderer/Renderer.hpp"
#include "glsandbox/Shader.hpp"
#include "glsandbox/Texture.hpp"
#include "glsandbox/Transform3D.hpp"
#include "glsandbox/VertexArray.hpp"
#include "glsandbox/VertexBufferLayout.hpp"
#include "glsandbox/defs.hpp"

int main(void)
{
    const Ref<Application> app = Application::GetInstance();
    Camera3D camera(Transform3D({0, 0, 3}, {0, -90, 0}));

    app->MouseEvent.Subscribe(MakeRef<Proc<GLFWwindow *, double, double>>(
        [&](GLFWwindow *window, double xPosition, double yPosition) {
            camera.Rotate(xPosition, yPosition);
        }));

    app->ScrollEvent.Subscribe(MakeRef<Proc<GLFWwindow *, double, double>>(
        [&](GLFWwindow *window, double xOffset, double yOffset) {
            camera.Zoom(xOffset, yOffset);
        }));

    const glm::vec4 clearColor = {.1f, .1f, .1f, 1.f};
    const std::vector<float> vertices = {
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //
        0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
        -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //

        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //
        0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
        -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //

        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, //
        -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f, //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f, //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f, //
        -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f, //
        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, //

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //
        0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f, //
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, //
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, //
        0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f, //
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //

        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //
        0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
        -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //

        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f, //
        0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
        -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f  //
    };
    VertexBuffer vb(vertices);

    float ambientStrength = 0.1;
    float specularStrength = 0.5;
    int specularShininess = 32;

    VertexArray cubeVa;
    VertexBufferLayout cubeLayout;
    cubeLayout.PushFloat(3);
    cubeLayout.PushFloat(3);
    cubeVa.AddBuffer(vb, cubeLayout);

    glm::vec3 cubeColor = {0.7, 0.2, 0.2};
    glm::vec3 cubePosition = {5, -5, 5};

    Shader cubeShader =
        Shader::FromSourceFiles(ASSETS_DIR "/shaders/basic_vertex.glsl",
                                ASSETS_DIR "/shaders/basic_fragment.glsl");

    VertexArray lampVa;
    VertexBufferLayout lampLayout;
    lampLayout.PushFloat(3);
    lampLayout.PushFloat(3);
    lampVa.AddBuffer(vb, lampLayout);

    glm::vec3 lampColor(1, 1, 1);
    glm::vec3 lampPosition(1.2f, 1.0f, 2.0f);
    unsigned long lampSpeed = 5;

    Shader lampShader =
        Shader::FromSourceFiles(ASSETS_DIR "/shaders/light_vertex.glsl",
                                ASSETS_DIR "/shaders/light_fragment.glsl");

    bool guiWindow = true;
    FrameTimer frameTimer;

    Renderer::SetClearColor({.1, .1, .1, 1});

    while (!app->ShouldClose())
    {
        float deltaTime = frameTimer.Tick();

        camera.Update(app->GetWindow(), deltaTime);

        lampPosition.z += lampSpeed * deltaTime * std::sin(glfwGetTime());

        Renderer::BeginDraw(camera);
        Renderer::Clear();

        glm::mat4 projection = camera.GetProjectionMatrix();
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0);

        cubeShader.Bind();
        cubeShader.SetUniform("u_Color", cubeColor);
        cubeShader.SetUniform("u_LightColor", lampColor);
        cubeShader.SetUniform("u_LightPosition", lampPosition);
        cubeShader.SetUniform("u_AmbientStr", ambientStrength);
        cubeShader.SetUniform("u_SpecularStr", specularStrength);
        cubeShader.SetUniform("u_SpecularShininess", specularShininess);
        cubeShader.SetUniform("u_CameraPosition",
                              camera.GetTransform().Position);
        cubeShader.SetUniform(
            "u_Model",
            glm::scale(glm::translate(model, cubePosition), {20, 0.5, 20}));
        cubeShader.SetUniform("u_View", view);
        cubeShader.SetUniform("u_Projection", projection);
        cubeVa.Bind();
        GL_Call(glDrawArrays(GL_TRIANGLES, 0, 36));

        lampShader.Bind();
        lampShader.SetUniform("u_Transform",
                              projection * view *
                                  glm::translate(model, lampPosition));
        lampVa.Bind();
        GL_Call(glDrawArrays(GL_TRIANGLES, 0, 36));

        Renderer::EndDraw();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (guiWindow)
        {
            ImGui::Begin("Options", &guiWindow);
            ImGui::Text("GLFW Time: %lf", glfwGetTime());

            camera.DrawImGUI();
            ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0, 1);
            ImGui::SliderFloat("Specular Strength", &specularStrength, 0, 1);
            ImGui::SliderFloat3("Cube position", glm::value_ptr(cubePosition),
                                -10, 10);
            ImGui::SliderFloat3("Cube color", glm::value_ptr(cubeColor), 0, 1);
            ImGui::SliderFloat3("Lamp position", glm::value_ptr(lampPosition),
                                -10, 10);
            ImGui::SliderFloat3("Lamp color", glm::value_ptr(lampColor), 0, 1);
            ImGui::InputInt("Specular Shininess", &specularShininess);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        app->SwapBuffers();
        app->PollEvents();
    }

    return EXIT_SUCCESS;
}

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
        -0.5f, -0.5f, -0.5f, //
        0.5f,  -0.5f, -0.5f, //
        0.5f,  0.5f,  -0.5f, //
        0.5f,  0.5f,  -0.5f, //
        -0.5f, 0.5f,  -0.5f, //
        -0.5f, -0.5f, -0.5f, //

        -0.5f, -0.5f, 0.5f, //
        0.5f,  -0.5f, 0.5f, //
        0.5f,  0.5f,  0.5f, //
        0.5f,  0.5f,  0.5f, //
        -0.5f, 0.5f,  0.5f, //
        -0.5f, -0.5f, 0.5f, //

        -0.5f, 0.5f,  0.5f,  //
        -0.5f, 0.5f,  -0.5f, //
        -0.5f, -0.5f, -0.5f, //
        -0.5f, -0.5f, -0.5f, //
        -0.5f, -0.5f, 0.5f,  //
        -0.5f, 0.5f,  0.5f,  //

        0.5f,  0.5f,  0.5f,  //
        0.5f,  0.5f,  -0.5f, //
        0.5f,  -0.5f, -0.5f, //
        0.5f,  -0.5f, -0.5f, //
        0.5f,  -0.5f, 0.5f,  //
        0.5f,  0.5f,  0.5f,  //

        -0.5f, -0.5f, -0.5f, //
        0.5f,  -0.5f, -0.5f, //
        0.5f,  -0.5f, 0.5f,  //
        0.5f,  -0.5f, 0.5f,  //
        -0.5f, -0.5f, 0.5f,  //
        -0.5f, -0.5f, -0.5f, //

        -0.5f, 0.5f,  -0.5f, //
        0.5f,  0.5f,  -0.5f, //
        0.5f,  0.5f,  0.5f,  //
        0.5f,  0.5f,  0.5f,  //
        -0.5f, 0.5f,  0.5f,  //
        -0.5f, 0.5f,  -0.5f, //
    };
    VertexBuffer vb(vertices.data(), vertices.size() * sizeof(float));

    float ambientStrength = 0.1;

    VertexArray cubeVa;
    VertexBufferLayout cubeLayout;
    cubeLayout.Pushf(3);
    cubeVa.AddBuffer(vb, cubeLayout);

    glm::vec3 cubeColor = {0.7, 0.2, 0.2};
    glm::vec3 cubePosition = {5, 0, 0};

    Shader cubeShader =
        Shader::FromSourceFiles(ASSETS_DIR "/shaders/basic_vertex.glsl",
                                ASSETS_DIR "/shaders/basic_fragment.glsl");

    VertexArray lampVa;
    VertexBufferLayout lampLayout;
    lampLayout.Pushf(3);
    lampVa.AddBuffer(vb, lampLayout);

    glm::vec3 lightColor(1, 1, 1);
    glm::vec3 lampPosition(1.2f, 1.0f, 2.0f);

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

        Renderer::BeginDraw(camera);
        Renderer::Clear();

        glm::mat4 projection = camera.GetProjectionMatrix();
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0);

        cubeShader.Bind();
        cubeShader.SetUniform("u_Color", glm::vec3(.6, .1, .1));
        cubeShader.SetUniform("u_LightColor", lightColor);
        cubeShader.SetUniform("u_AmbientStr", ambientStrength);

        cubeShader.SetUniform("u_Transform",
                              projection * view *
                                  glm::translate(model, cubePosition));
        cubeVa.Bind();
        GL_Call(glDrawArrays(GL_TRIANGLES, 0, 36));

        lampShader.Bind();
        lampShader.SetUniform("u_Transform",
                              projection * view *
                                  glm::translate(model, lampPosition));
        GL_Call(glDrawArrays(GL_TRIANGLES, 0, 36));

        Renderer::EndDraw();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (guiWindow)
        {
            ImGui::Begin("Options", &guiWindow);
            camera.DrawImGUI();
            ImGui::InputFloat("Ambient Strength", &ambientStrength);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        app->SwapBuffers();
        app->PollEvents();
    }

    return EXIT_SUCCESS;
}

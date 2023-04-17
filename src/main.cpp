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

    const glm::vec4 clearColor = {.1f, .1f, .1f, 1.f};
    const std::vector<float> vertices = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

    const std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};
    const std::vector<glm::vec3> positions{
        glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f)};

    Texture texture0(ASSETS_DIR "/textures/container.jpg");
    Texture texture1(ASSETS_DIR "/textures/awesomeface.png");

    VertexArray va;
    VertexBuffer vb(vertices.data(), vertices.size() * sizeof(float));
    IndexBuffer ib(indices);
    VertexBufferLayout layout;
    layout.Pushf(3);
    layout.Pushf(2);
    va.AddBuffer(vb, layout);

    Shader shader =
        Shader::FromSourceFiles(ASSETS_DIR "/shaders/basic_vertex.glsl",
                                ASSETS_DIR "/shaders/basic_fragment.glsl");
    shader.SetUniform("u_Texture0", 0);
    shader.SetUniform("u_Texture1", 1);
    shader.SetUniform("u_Color", glm::vec3(0.9, 0.1, 0.1));

    // ImGUI State
    bool guiWindow = true;

    FrameTimer frameTimer;

    app->MouseEvent.Subscribe(MakeRef<Proc<GLFWwindow *, double, double>>(
        [&](GLFWwindow *window, double xPosition, double yPosition) {
            camera.Rotate(xPosition, yPosition);
        }));

    app->ScrollEvent.Subscribe(MakeRef<Proc<GLFWwindow *, double, double>>(
        [&](GLFWwindow *window, double xOffset, double yOffset) {
            camera.Zoom(xOffset, yOffset);
        }));

    Renderer::SetClearColor({.1, .1, .1, 1});

    while (!app->ShouldClose())
    {
        float deltaTime = frameTimer.Tick();

        camera.Update(app->GetWindow(), deltaTime);

        Renderer::BeginDraw(camera);
        Renderer::Clear();

        va.Bind();
        ib.Bind();
        texture0.BindTo(0);
        texture1.BindTo(1);
        shader.Bind();

        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();

        for (uint64_t i = 0; i < positions.size(); ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            float angle = 20.0f * i;
            model =
                glm::rotate(model, glm::radians(angle) + (float)(glfwGetTime()),
                            glm::vec3(1.0f, 0.3f, 0.5f));

            shader.SetUniform("u_Transform", projection * view * model);
            GL_Call(glDrawArrays(GL_TRIANGLES, 0, 36));
        }

        Renderer::EndDraw();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (guiWindow)
        {
            ImGui::Begin("Options", &guiWindow);
            camera.DrawImGUI();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        app->SwapBuffers();
        app->PollEvents();
    }

    return EXIT_SUCCESS;
}

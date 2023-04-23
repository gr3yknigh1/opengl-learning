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
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  //
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   //
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   //
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //
                                                            //
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   //
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,    //
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,     //
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,     //
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,    //
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   //
                                                            //
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   //
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   //
                                                            //
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     //
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    //
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   //
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   //
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    //
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     //
                                                            //
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, //
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  //
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   //
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   //
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, //
                                                            //
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   //
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,    //
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,     //
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,     //
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,    //
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f    //
    };
    VertexBuffer vb(vertices);
    VertexBufferLayout layout;
    layout.PushFloat(3);
    layout.PushFloat(3);
    layout.PushFloat(2);

    VertexArray cubeVa;
    cubeVa.AddBuffer(vb, layout);

    glm::vec3 cubeColor = {0.7, 0.2, 0.2};
    glm::vec3 cubePosition = {5, 0, 0};

    Shader cubeShader =
        Shader::FromSourceFiles(ASSETS_DIR "/shaders/basic_vertex.glsl",
                                ASSETS_DIR "/shaders/basic_fragment.glsl");

    VertexArray lampVa;
    lampVa.AddBuffer(vb, layout);

    glm::vec3 lightColor(1, 1, 1);
    glm::vec3 lampPosition(1.2f, 1.0f, 2.0f);
    unsigned long lampSpeed = 5;

    Shader lampShader =
        Shader::FromSourceFiles(ASSETS_DIR "/shaders/light_vertex.glsl",
                                ASSETS_DIR "/shaders/light_fragment.glsl");

    bool guiWindow = true;
    bool moveLamp = false;
    FrameTimer frameTimer;

    Texture cubeTexture(ASSETS_DIR "/textures/container2.png");
    Texture cubeSpecMap(ASSETS_DIR "/textures/container2_specular.png");

    Renderer::SetClearColor({.1, .1, .1, 1});
    while (!app->ShouldClose())
    {
        float deltaTime = frameTimer.Tick();

        camera.Update(app->GetWindow(), deltaTime);

        lampPosition.z +=
            (lampSpeed * deltaTime * std::sin(glfwGetTime())) * (int)moveLamp;

        Renderer::BeginDraw(camera);
        Renderer::Clear();

        glm::mat4 projection = camera.GetProjectionMatrix();
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0);

        float ambientStr = 0.2f;
        float diffuseStr = 0.5f;

        glm::vec3 diffuseColor = lightColor * glm::vec3(diffuseStr);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(ambientStr);

        glm::vec3 lightAmbient = ambientColor;
        glm::vec3 lightDiffuse = diffuseColor;
        glm::vec3 lightSpecular = {1.0f, 1.0f, 1.0f};

        glm::vec3 materialAmbient = {1.0f, 0.5f, 0.31f};
        glm::vec3 materialDiffuse = {1.0f, 0.5f, 0.31f};
        glm::vec3 materialSpecular = {0.5f, 0.5f, 0.5f};

        float shininess = 32.0f;

        cubeShader.Bind();
        cubeShader.SetUniform("light.position", lampPosition);
        cubeShader.SetUniform("camera.position",
                              camera.GetTransform().Position);

        // cubeShader.SetUniform("material.ambient", materialAmbient);
        cubeTexture.BindTo(0);
        cubeShader.SetUniform("material.diffuse", 0);
        // cubeShader.SetUniform("material.specular", materialSpecular);
        cubeSpecMap.BindTo(1);
        cubeShader.SetUniform("material.specular", 1);
        cubeShader.SetUniform("material.shininess", shininess);

        cubeShader.SetUniform("light.ambient", lightAmbient);
        cubeShader.SetUniform("light.diffuse", lightDiffuse);
        cubeShader.SetUniform("light.specular", lightSpecular);

        cubeShader.SetUniform(
            "u_Model",
            glm::scale(glm::translate(model, cubePosition), {1, 1, 1}));
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
            ImGui::SliderFloat3("Cube position", glm::value_ptr(cubePosition),
                                -10, 10);
            ImGui::SliderFloat3("Lamp position", glm::value_ptr(lampPosition),
                                -10, 10);
            ImGui::Checkbox("Move Lamp", &moveLamp);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        app->SwapBuffers();
        app->PollEvents();
    }

    return EXIT_SUCCESS;
}

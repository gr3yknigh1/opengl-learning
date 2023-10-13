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
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "BEngine/BEngine.hpp"

#include "BEngine/Core/Application.hpp"
#include "BEngine/Core/FrameTimer.hpp"
#include "BEngine/Core/Transform3D.hpp"

#include "BEngine/Render/Camera3D.hpp"
#include "BEngine/Render/Renderer.hpp"

#include "BEngine/Render/GL/IndexBuffer.hpp"
#include "BEngine/Render/GL/Shader.hpp"
#include "BEngine/Render/GL/Utils.hpp"

#include "BEngine/Render/Model/Model.hpp"

#include "BEngine/Render/GL/Texture.hpp"
#include "BEngine/Render/GL/VertexArray.hpp"
#include "BEngine/Render/GL/VertexBufferLayout.hpp"

int main(void)
{
    using namespace BE::Core;
    using namespace BE::Core::Memory;
    using namespace BE::Render;

    const auto app = Application::GetInstance();

    Camera3D camera(Transform3D({0, 0, 3}, {0, -90, 0}));

    app->MouseEvent.Subscribe(MakeRef<Proc<GLFWwindow *, double, double>>(
        [&](GLFWwindow *window, double xPosition, double yPosition) { camera.Rotate(xPosition, yPosition); }));

    app->ScrollEvent.Subscribe(MakeRef<Proc<GLFWwindow *, double, double>>(
        [&](GLFWwindow *window, double xOffset, double yOffset) { camera.Zoom(xOffset, yOffset); }));

    const glm::vec4 clearColor = {.1f, .1f, .1f, 1.f};

    // const std::vector<float> vertices = {
    //     // positions          // normals           // texture coords
    //     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //
    //     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  //
    //     0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   //
    //     0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   //
    //     -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  //
    //     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //
    //                                                         //
    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   //
    //     0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,    //
    //     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,     //
    //     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,     //
    //     -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,    //
    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   //
    //                                                         //
    //     -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   //
    //     -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  //
    //     -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //
    //     -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //
    //     -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  //
    //     -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   //
    //                                                         //
    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     //
    //     0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    //
    //     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   //
    //     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   //
    //     0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    //
    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     //
    //                                                         //
    //     -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, //
    //     0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  //
    //     0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   //
    //     0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   //
    //     -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  //
    //     -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, //
    //                                                         //
    //     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   //
    //     0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,    //
    //     0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,     //
    //     0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,     //
    //     -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,    //
    //     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f    //
    // };
    // VertexBuffer vb(vertices);
    // VertexBufferLayout layout;
    // layout.PushFloat(3);
    // layout.PushFloat(3);
    // layout.PushFloat(2);

    // VertexArray cubeVa;
    // cubeVa.AddBuffer(vb, layout);

    // glm::vec3 cubeColor = {0.7, 0.2, 0.2};

    glm::vec3 cubePosition = {0, 0, 0};

    // std::vector<glm::vec3> cubePositions = {
    //     glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
    //     glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    //     glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    //     glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    //     glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    Shader cubeShader =
        Shader::FromSourceFiles(ASSETS_DIR "/shaders/model-vs.glsl", ASSETS_DIR "/shaders/model-fs.glsl");

    // VertexArray lampVa;
    // lampVa.AddBuffer(vb, layout);
    //
    // glm::vec3 lightColor(1, 1, 1);
    // glm::vec3 lampPosition(1.2f, 1.0f, 2.0f);
    // uint64_t lampSpeed = 5;

    // Shader lampShader =
    //     Shader::FromSourceFiles(ASSETS_DIR "/shaders/light_vertex.glsl",
    //                             ASSETS_DIR "/shaders/light_fragment.glsl");

    bool guiWindow = true;
    // bool moveLamp = false;
    FrameTimer frameTimer;

    // Texture cubeTexture(ASSETS_DIR "/textures/container2.png");
    // Texture cubeSpecMap(ASSETS_DIR "/textures/container2_specular.png");
    // Texture emissionMap(ASSETS_DIR "/textures/matrix.jpg");

    Renderer::SetClearColor({.1, .1, .1, 1});

    Model cubeModel(ASSETS_DIR "/meshes/ak_47.obj");

    uint64_t index = 0;

    while (!app->ShouldClose())
    {
        float deltaTime = frameTimer.Tick();
        double time = glfwGetTime();

        camera.Update(app->GetWindow(), deltaTime);

        // lampPosition.z +=
        //     (lampSpeed * deltaTime * std::sin(time)) * (int)moveLamp;

        Renderer::BeginDraw(camera);
        Renderer::Clear();

        glm::mat4 projection = camera.GetProjectionMatrix();
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0);

        // float ambientStr = 0.2f;
        // float diffuseStr = 0.5f;

        // glm::vec3 diffuseColor = lightColor * glm::vec3(diffuseStr);
        // glm::vec3 ambientColor = diffuseColor * glm::vec3(ambientStr);

        // glm::vec3 lightDirection = {-0.2f, -1.0f, -0.3f};
        // glm::vec3 lightAmbient = ambientColor;
        // glm::vec3 lightDiffuse = diffuseColor;
        // glm::vec3 lightSpecular = {1.0f, 1.0f, 1.0f};

        // glm::vec3 materialAmbient = {1.0f, 0.5f, 0.31f};
        // glm::vec3 materialDiffuse = {1.0f, 0.5f, 0.31f};
        // glm::vec3 materialSpecular = {0.5f, 0.5f, 0.5f};

        // float shininess = 32.0f;

        // cubeVa.Bind();
        cubeShader.Bind();
        // cubeShader.SetUniform("camera.position",
        //                       camera.GetTransform().Position);

        // cubeTexture.BindTo(0);
        // cubeShader.SetUniform("material.diffuse", 0);
        // cubeSpecMap.BindTo(1);
        // cubeShader.SetUniform("material.specular", 1);
        // emissionMap.BindTo(2);
        // cubeShader.SetUniform("material.emission", 2);
        // cubeShader.SetUniform("material.shininess", shininess);

        // cubeShader.SetUniform("light.direction", lightDirection);
        // cubeShader.SetUniform("light.ambient", lightAmbient);
        // cubeShader.SetUniform("light.diffuse", lightDiffuse);
        // cubeShader.SetUniform("light.specular", lightSpecular);

        // cubeShader.SetUniform("world.time", (float)time);

        cubeShader.SetUniform("u_View", view);
        cubeShader.SetUniform("u_Projection", projection);
        cubeShader.SetUniform("u_Model", glm::translate(model, cubePosition));
        cubeModel.Draw(cubeShader);

        // for (uint64_t i = 0; i < cubePositions.size(); ++i)
        // {
        //     glm::mat4 cubeModel = model;
        //     cubeModel =
        //         glm::rotate(cubeModel, 20.0f * i, glm::vec3(1.0f, 0.0f,
        //         0.0f));
        //     cubeModel = glm::scale(cubeModel, {1, 1, 1});
        // cubeModel = glm::translate(cubeModel, cubePositions[i]);
        //     cubeShader.SetUniform("u_Model", cubeModel);
        //     GL_Call(glDrawArrays(GL_TRIANGLES, 0, 36));
        // }

        // lampShader.Bind();
        // lampShader.SetUniform("u_Transform",
        //                       projection * view *
        //                           glm::translate(model, lampPosition));
        // lampVa.Bind();
        // GL_Call(glDrawArrays(GL_TRIANGLES, 0, 36));

        Renderer::EndDraw();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (guiWindow)
        {
            ImGui::Begin("Options", &guiWindow);
            ImGui::Text("GLFW Time: %lf", glfwGetTime());
            camera.DrawImGUI();

            // ImGui::SliderInt("Cube Position Index", (int *)(&index), 0,
            //                  cubePositions.size() - 1);
            // ImGui::SliderFloat3("Cube position",
            //                     glm::value_ptr(cubePositions[index]), -10,
            //                     10);
            // ImGui::SliderFloat3("Lamp position",
            // glm::value_ptr(lampPosition),
            //                     -10, 10);
            // ImGui::Checkbox("Move Lamp", &moveLamp);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        app->SwapBuffers();
        app->PollEvents();
    }

    return EXIT_SUCCESS;
}

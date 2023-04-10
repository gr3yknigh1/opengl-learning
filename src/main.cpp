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

#include "glsandbox/GLUtils.hpp"
#include "glsandbox/IndexBuffer.hpp"
#include "glsandbox/Shader.hpp"
#include "glsandbox/Texture.hpp"
#include "glsandbox/VertexArray.hpp"
#include "glsandbox/VertexBufferLayout.hpp"
#include "glsandbox/defs.hpp"

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const char *GLFW_ErrorCodeDispatch(int errorCode)
{
    switch (errorCode)
    {
    case GLFW_NOT_INITIALIZED:
        return STRINGIFY(GLFW_NOT_INITIALIZED);
    case GLFW_NO_CURRENT_CONTEXT:
        return STRINGIFY(GLFW_NO_CURRENT_CONTEXT);
    case GLFW_INVALID_ENUM:
        return STRINGIFY(GLFW_INVALID_ENUM);
    case GLFW_OUT_OF_MEMORY:
        return STRINGIFY(GLFW_OUT_OF_MEMORY);
    case GLFW_API_UNAVAILABLE:
        return STRINGIFY(GLFW_API_UNAVAILABLE);
    case GLFW_VERSION_UNAVAILABLE:
        return STRINGIFY(GLFW_VERSION_UNAVAILABLE);
    case GLFW_PLATFORM_ERROR:
        return STRINGIFY(GLFW_PLATFORM_ERROR);
    case GLFW_FORMAT_UNAVAILABLE:
        return STRINGIFY(GLFW_FORMAT_UNAVAILABLE);
    case GLFW_NO_WINDOW_CONTEXT:
        return STRINGIFY(GLFW_NO_WINDOW_CONTEXT);
    default:
        return "GLFW_UNHANDLED_ERROR";
    }

    return nullptr;
}

void GLFW_FrameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    GL_Call(glViewport(0, 0, width, height));
}

void GLFW_ErrorHandler(int errorCode, const char *description)
{
    std::printf("[GLFW]: %s - error code %d, description: '%s'\n",
                GLFW_ErrorCodeDispatch(errorCode), errorCode, description);
}

void GLFW_KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                      int mods)
{
    if ((key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS)
    {
        GL_Call(glfwSetWindowShouldClose(window, true));
    }
}

void PrintDebugInfo()
{
    const unsigned char *glVersion = nullptr;
    GL_CallO(glGetString(GL_VERSION), &glVersion);

    const unsigned char *glGlslVersion = nullptr;
    GL_CallO(glGetString(GL_SHADING_LANGUAGE_VERSION), &glGlslVersion);

    const unsigned char *glRendererVersion = nullptr;
    GL_CallO(glGetString(GL_RENDERER), &glRendererVersion);

    const unsigned char *glVendor = nullptr;
    GL_CallO(glGetString(GL_VENDOR), &glVendor);

    std::printf("[GLFW]: %s\n", glfwGetVersionString());
    std::printf("[GL]: OpenGL  : %s, GLSL: %s\n", glVersion, glGlslVersion);
    std::printf("[GL]: Renderer: %s\n", glRendererVersion);
    std::printf("[GL]: Vendor  : %s\n", glVendor);
}

int main(void)
{
    if (!glfwInit())
    {
        std::printf("[GLFW]: Error! Failed initialize glfw\n");
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // NOTE: For compatibility
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glm::vec2 windowSize = {900, 600};
    const char *title = "Triangle";
    GLFWwindow *window =
        glfwCreateWindow(windowSize.x, windowSize.y, title, nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        std::printf("[GLFW]: Error! Failed create window\n");
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(GLFW_ErrorHandler);
    glfwSetKeyCallback(window, GLFW_KeyCallback);
    glfwSetFramebufferSizeCallback(window, GLFW_FrameBufferSizeCallback);
    glfwSwapInterval(1);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        std::printf("[GLEW]: Error! Failed initialize glew\n");
        return EXIT_FAILURE;
    }

    PrintDebugInfo();

    GL_Call(glViewport(0, 0, windowSize.x, windowSize.y));
    GL_Call(glEnable(GL_BLEND));
    GL_Call(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_Call(glEnable(GL_DEPTH_TEST));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    const char *glslVersion = "#version 130";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    // ------------------------------------------------------ //

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
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    Texture texture0(ASSETS_DIR "/textures/container.jpg");
    Texture texture1(ASSETS_DIR "/textures/awesomeface.png");

    VertexArray va;
    VertexBuffer vb(vertices.data(), vertices.size() * sizeof(float));
    IndexBuffer ib(indices.data(), indices.size() * sizeof(unsigned int));
    VertexBufferLayout layout;
    layout.Pushf(3);
    layout.Pushf(2);
    va.AddBuffer(vb, layout);

    Shader shader =
        Shader::FromSourceFiles(ASSETS_DIR "/shaders/basic_vertex.glsl",
                                ASSETS_DIR "/shaders/basic_fragment.glsl");
    shader.SetUniform("u_Texture0", 0);
    shader.SetUniform("u_Texture1", 1);

    float fov = 45.0f;

    glm::mat4 model = glm::mat4(1.0f);
    model =
        glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection = glm::perspective(
        glm::radians(fov), windowSize.x / windowSize.y, 0.1f, 100.0f);

    glm::mat4 transformation = projection * view * model;
    shader.SetUniform("u_Transform", transformation);

#if 1
    GL_Call(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
#else
    GL_Call(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
#endif

    // ImGUI State
    bool guiWindow = true;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        float cameraSpeed = 2.5f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPosition += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPosition -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPosition -=
                glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPosition +=
                glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        GL_Call(glClearColor(clearColor.r, clearColor.b, clearColor.g,
                             clearColor.a));
        GL_Call(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        va.Bind();
        ib.Bind();
        texture0.BindTo(0);
        texture1.BindTo(1);
        shader.Bind();

        for (uint64_t i = 0; i < positions.size(); ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(
                model, glm::radians(angle) + (float)(glfwGetTime() * 0.01),
                glm::vec3(1.0f, 0.3f, 0.5f));

            projection = glm::perspective(
                glm::radians(fov), windowSize.x / windowSize.y, 0.1f, 100.0f);

            // glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
            // glm::vec3 cameraDirection =
            //     glm::normalize(cameraPosition - cameraTarget);
            // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
            // glm::vec3 cameraRight =
            //     glm::normalize(glm::cross(up, cameraDirection));
            // glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
            //
            // const float radius = 10.0f;
            // float camX = sin(glfwGetTime()) * radius;
            // float camZ = cos(glfwGetTime()) * radius;
            // view =
            //     glm::lookAt(glm::vec3(camX, 0.0, camZ),
            //                 glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0,
            //                 0.0));
            view = glm::lookAt(cameraPosition, cameraPosition + cameraFront,
                               cameraUp);

            shader.SetUniform("u_Transform", projection * view * model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // GL_Call(
        //     glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0));

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (guiWindow)
        {
            ImGui::Begin("Options", &guiWindow);
            ImGui::SliderFloat(
                "FOV", &fov, 0.0f,
                360.0f); // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::InputFloat3("Camera Position",
                               glm::value_ptr(cameraPosition));
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ------------------------------------------------------ //

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

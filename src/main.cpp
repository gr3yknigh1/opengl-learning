#include <cstdint>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define __DEBUG_MODE__
#define LOG(X) std::cout << X << '\n'
#define STRINGIFY(X) (#X)
#define ARRAY_COUNT(X) (sizeof((X)) / sizeof((X)[0]))

// TODO: Move to separate file
const char *VERTEX_SHADER_SOURCE =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *FRAGMENT_SHADER_SOURCE =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 0.7f);\n"
    "}\n\0";

/*
 *
 * Checkout all errors at <https://docs.gl/gl4/glGetError>
 * */
const char *GLEW_ErrorCodeDispatch(int errorCode)
{
    switch (errorCode)
    {
    case GL_NO_ERROR:
        return STRINGIFY(GL_NO_ERROR);
    case GL_INVALID_ENUM:
        return STRINGIFY(GL_INVALID_ENUM);
    case GL_INVALID_VALUE:
        return STRINGIFY(GL_INVALID_VALUE);
    case GL_INVALID_OPERATION:
        return STRINGIFY(GL_INVALID_OPERATION);
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return STRINGIFY(GL_INVALID_FRAMEBUFFER_OPERATION);
    case GL_OUT_OF_MEMORY:
        return STRINGIFY(GL_OUT_OF_MEMORY);
    case GL_STACK_UNDERFLOW:
        return STRINGIFY(GL_STACK_UNDERFLOW);
    case GL_STACK_OVERFLOW:
        return STRINGIFY(GL_STACK_OVERFLOW);
    default:
        return "GL_UNHANDLED_ERROR";
    }

    return nullptr;
}

void GL_ClearErrors()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

void GL_CheckErrors(const char *glFunctionName, const char *sourceFilePath,
                    uint32_t sourceLine)
{
    while (GLenum errorCode = glGetError())
    {
        std::printf("[GL]: %s - error code %d\n",
                    GLEW_ErrorCodeDispatch(errorCode), errorCode);
        std::printf("[GL]: Occured at %s::%d during %s call\n", sourceFilePath,
                    sourceLine, glFunctionName);
        std::exit(EXIT_FAILURE);
    }
}

#define GL_Call(__glExpr)                                                      \
    do                                                                         \
    {                                                                          \
        GL_ClearErrors();                                                      \
        (__glExpr);                                                            \
        GL_CheckErrors(#__glExpr, __FILE__, __LINE__);                         \
    } while (0)

#define GL_CallO(__glExpr, __outPtr)                                           \
    do                                                                         \
    {                                                                          \
        GL_ClearErrors();                                                      \
        *(__outPtr) = (__glExpr);                                              \
        GL_CheckErrors(#__glExpr, __FILE__, __LINE__);                         \
    } while (0)

// TODO: Print which file is occuring error
unsigned int GL_CompileShader(const char *shaderSource, int shaderType)
{
    int success;
    char shaderCompilationInfoLog[GL_INFO_LOG_LENGTH];

    unsigned int shaderId = 0;
    GL_CallO(glCreateShader(shaderType), &shaderId);
    GL_Call(glShaderSource(shaderId, 1, &shaderSource, nullptr));
    GL_Call(glCompileShader(shaderId));
    GL_Call(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success));

    if (!success)
    {
        GL_Call(glGetShaderInfoLog(shaderId, GL_INFO_LOG_LENGTH, nullptr,
                                   shaderCompilationInfoLog));
        std::printf("[GL]: Error during shader compilation: %s\n",
                    shaderCompilationInfoLog);
        std::exit(EXIT_FAILURE);
        return 0;
    }

    return shaderId;
}

unsigned int GL_LinkShaderProgram(unsigned int vertexShaderId,
                                  unsigned int fragmentShaderId)
{
    int success;
    char shaderCompilationInfoLog[GL_INFO_LOG_LENGTH];

    unsigned int shaderProgramId = 0;
    GL_CallO(glCreateProgram(), &shaderProgramId);

    GL_Call(glAttachShader(shaderProgramId, vertexShaderId));
    GL_Call(glAttachShader(shaderProgramId, fragmentShaderId));
    GL_Call(glLinkProgram(shaderProgramId));

    GL_Call(glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success));

    if (!success)
    {
        GL_Call(glGetProgramInfoLog(shaderProgramId, GL_INFO_LOG_LENGTH,
                                    nullptr, shaderCompilationInfoLog));
        std::exit(EXIT_FAILURE);
    }

    return shaderProgramId;
}

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
    glfwSwapInterval(true);

#ifdef __DEBUG_MODE__
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

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

    // ------------------------------------------------------ //

    glm::vec4 clearColor = {.1f, .1f, .1f, 1.f};
    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,

         0.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 0.0f,
         1.0f, 0.0f, 0.0f,
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        3, 4, 5,
    };

    uint32_t vbo = 0, vao = 0, ebo = 0;

    // NOTE: it must be before `glVertexAttribPointer` call
    GL_Call(glGenVertexArrays(1, &vao));
    GL_Call(glBindVertexArray(vao));

    GL_Call(glGenBuffers(1, &vbo));
    GL_Call(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_Call(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                         GL_STATIC_DRAW));

    GL_Call(glGenBuffers(1, &ebo));
    GL_Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL_Call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                         GL_STATIC_DRAW));

    GL_Call(
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0));
    GL_Call(glEnableVertexAttribArray(0));

    uint32_t vertexShader =
        GL_CompileShader(VERTEX_SHADER_SOURCE, GL_VERTEX_SHADER);
    uint32_t fragmentShader =
        GL_CompileShader(FRAGMENT_SHADER_SOURCE, GL_FRAGMENT_SHADER);

    uint32_t shaderProgram = GL_LinkShaderProgram(vertexShader, fragmentShader);

    GL_Call(glUseProgram(shaderProgram));
    GL_Call(glDeleteShader(vertexShader));
    GL_Call(glDeleteShader(fragmentShader));

#if 1
    GL_Call(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
#else
    GL_Call(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
#endif

    while (!glfwWindowShouldClose(window))
    {
        GL_Call(glClearColor(clearColor.r, clearColor.b, clearColor.g,
                             clearColor.a));
        GL_Call(glClear(GL_COLOR_BUFFER_BIT));

        GL_Call(glUseProgram(shaderProgram));
        GL_Call(glBindVertexArray(vao));
        GL_Call(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ------------------------------------------------------ //

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

#include "glsandbox/glutils.hpp"

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

#include "BEngine/Render/GL/Utils.hpp"

#include <stdexcept>

#include <fmt/format.h>

#include "BEngine/BEngine.hpp"

/*
 *
 * Checkout all errors at <https://docs.gl/gl4/glGetError>
 * */
const char *GLEW_ErrorCodeDispatch(int errorCode)
{
    switch (errorCode)
    {
    case GL_NO_ERROR:
        return BE_STRINGIFY(GL_NO_ERROR);
    case GL_INVALID_ENUM:
        return BE_STRINGIFY(GL_INVALID_ENUM);
    case GL_INVALID_VALUE:
        return BE_STRINGIFY(GL_INVALID_VALUE);
    case GL_INVALID_OPERATION:
        return BE_STRINGIFY(GL_INVALID_OPERATION);
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return BE_STRINGIFY(GL_INVALID_FRAMEBUFFER_OPERATION);
    case GL_OUT_OF_MEMORY:
        return BE_STRINGIFY(GL_OUT_OF_MEMORY);
    case GL_STACK_UNDERFLOW:
        return BE_STRINGIFY(GL_STACK_UNDERFLOW);
    case GL_STACK_OVERFLOW:
        return BE_STRINGIFY(GL_STACK_OVERFLOW);
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
        throw std::runtime_error(fmt::format(
            "[GL]: {} - error code {}\n[GL]: Occured at {}::{} during {} call",
            GLEW_ErrorCodeDispatch(errorCode), errorCode, sourceFilePath,
            sourceLine, glFunctionName));
    }
}

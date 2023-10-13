#ifndef BENGINE_RENDER_GL_GLUTILS_HPP
#define BENGINE_RENDER_GL_GLUTILS_HPP

#include <cstdint>
#include <iostream>

#include <GL/glew.h>

const char *GLEW_ErrorCodeDispatch(int errorCode);

void GL_ClearErrors();
void GL_CheckErrors(const char *glFunctionName, const char *sourceFilePath,
                    uint32_t sourceLine);

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

#endif // BENGINE_RENDER_GL_GLUTILS_HPP

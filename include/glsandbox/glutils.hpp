#ifndef GLUTILS_HPP_H_
#define GLUTILS_HPP_H_

#include "glsandbox/defs.hpp"
#include <GL/glew.h>
#include <iostream>

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

#endif // GLUTILS_HPP_H_

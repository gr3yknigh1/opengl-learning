#ifndef BENGINE_RENDER_GL_VERTEX_HPP
#define BENGINE_RENDER_GL_VERTEX_HPP

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 NormalVector;
    glm::vec2 TextureCoordinates;
};

#endif // BENGINE_RENDER_GL_VERTEX_HPP

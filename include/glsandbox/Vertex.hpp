#ifndef VERTEX_HPP_
#define VERTEX_HPP_

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 NormalVector;
    glm::vec2 TextureCoordinates;
};

#endif // VERTEX_HPP_

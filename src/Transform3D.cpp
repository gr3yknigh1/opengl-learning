#include "glsandbox/Transform3D.hpp"

Transform3D::Transform3D(const glm::vec3 &position, const glm::vec3 &rotation)
    : Position(position), Rotation(rotation)
{
}

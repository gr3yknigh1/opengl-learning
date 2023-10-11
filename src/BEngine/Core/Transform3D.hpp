#ifndef TRANSFORM3D_HPP_
#define TRANSFORM3D_HPP_

#include <glm/glm.hpp>

struct Transform3D
{
    Transform3D(const glm::vec3 &position, const glm::vec3 &rotation);

    glm::vec3 Position;

    // NOTE: x - pitch, y - yaw, z - roll
    glm::vec3 Rotation;
};

#endif // TRANSFORM3D_HPP_

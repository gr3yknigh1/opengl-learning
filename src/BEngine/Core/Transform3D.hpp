#ifndef BENGINE_CORE_TRANSFORM3D_HPP
#define BENGINE_CORE_TRANSFORM3D_HPP

#include <glm/glm.hpp>

struct Transform3D
{
    Transform3D(const glm::vec3 &position, const glm::vec3 &rotation);

    glm::vec3 Position;

    // NOTE: x - pitch, y - yaw, z - roll
    glm::vec3 Rotation;
};

#endif // BENGINE_CORE_TRANSFORM3D_HPP

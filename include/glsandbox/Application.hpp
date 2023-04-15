#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include <glm/glm.hpp>
#include <GL/glew.h>

class Application
{
public:
    static inline bool IsCursorEnabled = true;
    static inline const glm::vec2 WindowSize = {900, 600};
};

#endif // APPLICATION_HPP_

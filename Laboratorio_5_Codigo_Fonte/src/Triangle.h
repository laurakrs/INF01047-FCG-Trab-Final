#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>

class Triangle
{
public:
    // Constructors
    Triangle();
    Triangle(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3);
    glm::vec4 vertices[3];
};

#endif // TRIANGLE_H

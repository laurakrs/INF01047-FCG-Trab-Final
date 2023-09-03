#include "Triangle.h"

Triangle::Triangle() {
}

Triangle::Triangle(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3) {
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
}

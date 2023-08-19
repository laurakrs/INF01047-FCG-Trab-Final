#pragma once

#include <vector>
#include <limits>
#include <tiny_obj_loader.h>
#include <glm/glm.hpp>

struct BoundingBox {
    glm::vec3 min;
    glm::vec3 max;
};

BoundingBox computeBoundingBox(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape);

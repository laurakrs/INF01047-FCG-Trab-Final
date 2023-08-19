#include "BoundingBox.h"

BoundingBox computeBoundingBox(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape) {
    BoundingBox bbox;
    bbox.min = glm::vec3(std::numeric_limits<float>::max());
    bbox.max = glm::vec3(std::numeric_limits<float>::lowest());

    for (size_t i = 0; i < shape.mesh.indices.size(); ++i) {
        tinyobj::index_t idx = shape.mesh.indices[i];
        float vx = attrib.vertices[3*idx.vertex_index + 0];
        float vy = attrib.vertices[3*idx.vertex_index + 1];
        float vz = attrib.vertices[3*idx.vertex_index + 2];

        glm::vec3 v(vx, vy, vz);

        bbox.min = glm::min(bbox.min, v);
        bbox.max = glm::max(bbox.max, v);
    }
    return bbox;
}


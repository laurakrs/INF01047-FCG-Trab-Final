// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers da biblioteca para carregar modelos obj
#include <tiny_obj_loader.h>

// Given a tinyobj mesh, calculate the bounding box
// void calculateBoundingBox(const tinyobj::mesh_t& mesh, glm::vec3& min, glm::vec3& max) {
//     // Initialize min and max to the first vertex position
//     min = max = glm::vec3(mesh.vertices[0], mesh.positions[1], mesh.positions[2]);

//     // Loop through all vertex positions and update min and max
//     for (size_t i = 0; i < mesh.positions.size(); i += 3) {
//         glm::vec3 pos(mesh.positions[i], mesh.positions[i+1], mesh.positions[i+2]);
//         min = glm::min(min, pos);
//         max = glm::max(max, pos);
//     }
// }
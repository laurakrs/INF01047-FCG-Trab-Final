#include "ObjectInstance.h"

// Initialization of the static variable
int ObjectInstance::next_object_id = 0;

// Default Constructor definition
ObjectInstance::ObjectInstance()
    : object_name(""), model_matrix(glm::mat4(1.0f)), object_id(-1) {
    // Initialization (the values provided here are just placeholders, adjust as necessary)
}

// Constructor definition
ObjectInstance::ObjectInstance(const std::string& name, const glm::mat4& matrix, const int& id)
    : object_name(name), model_matrix(matrix), object_id(id) {
    // Add this instance to the global map
    g_ObjectInstances[object_id] = *this;

    // Assuming you have some initial values for the bounding box before any transformation
    glm::vec3 initial_min = g_VirtualScene[name].bbox_min;
    glm::vec3 initial_max = g_VirtualScene[name].bbox_max;
    UpdateBoundingBox(initial_min, initial_max);
}

// // Constructor definition
// ObjectInstance::ObjectInstance(const std::string& name, const glm::mat4& matrix, const int& object_id)
//     : object_name(name), model_matrix(matrix), object_id(next_object_id++) {
//     // Add this instance to the global map
//     g_ObjectInstances[object_id] = *this;
// }

// Definition of the global variable
std::map<int, ObjectInstance> g_ObjectInstances;

void ObjectInstance::UpdateBoundingBox(const glm::vec3& initial_min, const glm::vec3& initial_max)
{
    // Cria todos os cantos de um cubóide que representa uma bounding box axis aligned
    std::vector<glm::vec3> corners = {
        initial_min,
        glm::vec3(initial_max.x, initial_min.y, initial_min.z),
        glm::vec3(initial_min.x, initial_max.y, initial_min.z),
        glm::vec3(initial_max.x, initial_max.y, initial_min.z),
        glm::vec3(initial_min.x, initial_min.y, initial_max.z),
        glm::vec3(initial_max.x, initial_min.y, initial_max.z),
        glm::vec3(initial_min.x, initial_max.y, initial_max.z),
        initial_max
    };

    bbox_min = glm::vec3(FLT_MAX);
    bbox_max = glm::vec3(-FLT_MAX);

    // Aplica a transformada e itera entre os cantos pra achar o mínimo e o máximo da bounding box
    for (const auto& corner : corners)
    {
        glm::vec4 transformed_corner = model_matrix * glm::vec4(corner, 1.0f);

        bbox_min = glm::min(bbox_min, glm::vec3(transformed_corner));
        bbox_max = glm::max(bbox_max, glm::vec3(transformed_corner));
    }
}

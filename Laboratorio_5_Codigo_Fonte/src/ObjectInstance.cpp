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
}

// // Constructor definition
// ObjectInstance::ObjectInstance(const std::string& name, const glm::mat4& matrix, const int& object_id)
//     : object_name(name), model_matrix(matrix), object_id(next_object_id++) {
//     // Add this instance to the global map
//     g_ObjectInstances[object_id] = *this;
// }

// Definition of the global variable
std::map<int, ObjectInstance> g_ObjectInstances;

#include "ObjectInstance.h"

// Initialization of the static variable
int ObjectInstance::next_object_id = 0;

// Default Constructor definition
ObjectInstance::ObjectInstance()
    : object_name(""), model_matrix(glm::mat4(1.0f)), sceneObject_id(-1) {
}

// Constructor definition
ObjectInstance::ObjectInstance(const std::string& name, const glm::mat4& matrix, const int& sceneObject_id)
    : object_name(name), model_matrix(matrix), sceneObject_id(sceneObject_id) {

    instance_id = next_object_id++;
    g_ObjectInstances[instance_id] = *this; 
}

// Definition of the global variable
std::map<int, ObjectInstance> g_ObjectInstances;

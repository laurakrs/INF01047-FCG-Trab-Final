#include "ObjectInstance.h"

// Iinicialização da variável estática
int ObjectInstance::next_object_id = 0;

glm::mat4 ObjectInstance::identityMatrix = glm::mat4(
        1.0f , 0.0f , 0.0f , 0.0f , // LINHA 1
        0.0f , 1.0f , 0.0f , 0.0f , // LINHA 2
        0.0f , 0.0f , 1.0f , 0.0f , // LINHA 3
        0.0f , 0.0f , 0.0f , 1.0f   // LINHA 4
    );

ObjectInstance::ObjectInstance()
    : object_name(""), model_matrix(glm::mat4(1.0f)), sceneObject_id(-1) {
}

ObjectInstance::ObjectInstance(const std::string& name, const glm::mat4& matrix, const int& sceneObject_id)
    : object_name(name), model_matrix(matrix), sceneObject_id(sceneObject_id) {

    instance_id = next_object_id++;
    g_ObjectInstances[instance_id] = *this; 
}

ObjectInstance::ObjectInstance(const std::string& name, const int& sceneObject_id)
    : object_name(name), model_matrix(identityMatrix), sceneObject_id(sceneObject_id) {

    instance_id = next_object_id++;
    g_ObjectInstances[instance_id] = *this; 
}

// Definition of the global variable
std::map<int, ObjectInstance> g_ObjectInstances;

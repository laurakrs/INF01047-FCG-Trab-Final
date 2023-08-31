#pragma once

#include <string>
#include <map>
// #include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "SceneObject.h"

struct ObjectInstance
{
    glm::mat4  model_matrix;    // Transformada da instância do objeto
    int       object_id;        // Id do objeto
    std::string object_name;    // O nome do objeto. Ex:"the_sphere", "the_bunny", etc.
    glm::vec3    bbox_min;      // Ponto mínimo da bounding box da instância do objeto
    glm::vec3    bbox_max;      // Ponto máximo da bounding box da instância do objeto

    static int next_object_id;  // Static variable that acts as a counter for the next available ID

    // Constructors
    ObjectInstance();  // construtor default para o map funcionar
    ObjectInstance(const std::string& name, const glm::mat4& matrix, const int& id);

    void UpdateBoundingBox(const glm::vec3& initial_min, const glm::vec3& initial_max);
};

extern std::map<int, ObjectInstance> g_ObjectInstances; // where int can be a unique identifier for each instance.


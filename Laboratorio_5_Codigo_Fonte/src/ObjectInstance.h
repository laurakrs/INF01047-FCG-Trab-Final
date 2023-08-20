#pragma once

#include <string>
#include <map>
// #include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct ObjectInstance
{
    glm::mat4  model_matrix;    // Transformation matrix for the instance
    int       object_id;        // This can be SPHERE, BUNNY, PLANE, etc.
    std::string object_name;    // This can be "the_sphere", "the_bunny", etc.

    static int next_object_id;  // Static variable that acts as a counter for the next available ID

    // Constructors
    ObjectInstance();  // construtor default para o map funcionar
    ObjectInstance(const std::string& name, const glm::mat4& matrix, const int& id);
};

extern std::map<int, ObjectInstance> g_ObjectInstances; // where int can be a unique identifier for each instance.
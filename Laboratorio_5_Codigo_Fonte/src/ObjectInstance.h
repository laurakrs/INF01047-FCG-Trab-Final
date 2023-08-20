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
};

std::map<int, ObjectInstance> g_ObjectInstances; // where int can be a unique identifier for each instance.
#pragma once

#include <glm/glm.hpp>
#include "globals.h"
#include "SceneInformation.h"

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Definição da struct Ray
struct CursorRay {
    glm::vec4 startPoint;
    glm::vec4 direction;
};

// Raio projetado com o clique do mouse
extern CursorRay g_cursorRay;

CursorRay ComputeRayFromMouse(GLFWwindow* window, const glm::mat4& projMatrix, const glm::mat4& viewMatrix);
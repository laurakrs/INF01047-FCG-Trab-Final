#pragma once

#include <string>
#include <map>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

// Definimos uma estrutura que armazenará dados necessários para renderizar
// cada objeto da cena virtual.
struct SceneObject
{
    std::string  name;
    size_t       first_index;
    size_t       num_indices;
    GLenum       rendering_mode;
    GLuint       vertex_array_object_id;
    glm::vec3    bbox_min;
    glm::vec3    bbox_max;

    std::vector<glm::vec3> bbox_vertices;
    std::vector<GLuint> bbox_indices;
};

// A cena virtual é uma lista de objetos nomeados, guardados em um dicionário
// (map).  Veja dentro da função BuildTrianglesAndAddToVirtualScene() como que são incluídos
// objetos dentro da variável g_VirtualScene, e veja na função main() como
// estes são acessados.
extern std::map<std::string, SceneObject> g_VirtualScene;

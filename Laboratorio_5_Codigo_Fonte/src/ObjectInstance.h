#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>
#include <vector>
#include "SceneObject.h"

struct ObjectInstance
{
    glm::mat4  model_matrix;        // Transformada da instância do objeto
    int       sceneObject_id;       // Id do objeto
    std::string object_name;        // O nome do objeto. Ex:"the_sphere", "the_bunny", etc.
    int         instance_id;        // Id do objeto

    static int next_object_id;      // Variável estática que é incrementada a cada objeto criado
    static glm::mat4 identityMatrix;// Matriz identidade utilizada para inicializar a model_matrix (importar matrices.h dava conflito)

    // Construtores
    ObjectInstance();  // construtor default para o map funcionar
    ObjectInstance(const std::string& name, const int& sceneObject_id); // Construtor usado na interface
    ObjectInstance(const std::string& name, const glm::mat4& matrix, const int& sceneObject_id);
};

extern std::map<int, ObjectInstance> g_ObjectInstances;


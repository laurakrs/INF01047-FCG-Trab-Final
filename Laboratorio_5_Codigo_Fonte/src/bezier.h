// Arquivo separado para a curva de bezier
#include <cmath>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

// 4 pontos de controle
glm::vec4 p1;
glm::vec4 p2;
glm::vec4 p3;
glm::vec4 p4;
float t; 

glm::vec4 point;

glm::vec4 bezierCurve(float t, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4);

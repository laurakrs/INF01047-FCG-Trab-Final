// Arquivo separado para a curva de bezier
#include <cmath>
#include <glm/glm.hpp>

// 4 pontos de controle
glm::vec3 p1;
glm::vec3 p2;
glm::vec3 p3;
glm::vec3 p4;

glm::vec3 point;

glm::vec3 bezierCurve(float t, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);

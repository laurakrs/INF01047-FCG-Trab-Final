// Arquivo separado para a curva de bezier
#include "bezier.h"


// bezier curve - cubica
glm::vec4 bezierCurve(float t_passed, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4){

    //float t = (t_passed/10.0f) - floor(t_passed/10.0f);
    float t = fmod(t_passed, 10.0f) / 10.0f; // Normalizado
    glm::vec4 point = (1.0f-t)*(1.0f-t)*(1.0f-t)*p1 +
            3.0f*t*(1.0f-t)*(1.0f-t)*p2 +
            3.0f*t*t*(1.0f-t)*p3 +
            t*t*t*p4;

    return point;
}

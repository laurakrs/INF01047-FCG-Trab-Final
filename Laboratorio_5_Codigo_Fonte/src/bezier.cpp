// Arquivo separado para a curva de bezier
#include "bezier.h"


// bezier curve - cubica
glm::vec4 bezierCurve(float t_passed, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4){

    float t = (t_passed/10.0f) - floor(t_passed/10.0f);
    glm::vec4 point = (1-t)*(1-t)*(1-t)*p1 +
            3*t*(1-t)*(1-t)*p2 +
            3*t*t*(1-t)*p3 +
            t*t*t*p4;

    return point;
}

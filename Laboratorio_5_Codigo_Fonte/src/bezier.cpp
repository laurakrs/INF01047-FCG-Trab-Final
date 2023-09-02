// Arquivo separado para a curva de bezier
#include "bezier.h"


// bezier curve
glm::vec3 bezierCurve( float t, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4){

   glm::vec3 point = pow(1.0f-t,3.0f)*p1 +
                    (3.0f * t * pow((1.0f-t),2.0f))*p2 +
                    ((3.0f*pow(t,2.0f) * (1.0f-t))*p3) +
                    pow(t,3.0f)*p4;
}
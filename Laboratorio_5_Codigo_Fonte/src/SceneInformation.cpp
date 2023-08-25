#include "SceneInformation.h"

glm::mat4 SceneInformation::view = glm::mat4(1.0f);                 // identity matrix as default value
glm::mat4 SceneInformation::projection = glm::mat4(1.0f);          // identity matrix as default value
glm::vec4 SceneInformation::camera_position_c = glm::vec4(0.0f);   // default value
glm::vec4 SceneInformation::camera_view_vector = glm::vec4(0.0f);  // default value
glm::vec4 SceneInformation::camera_up_vector = glm::vec4(0.0f,1.0f,0.0f,0.0f);    // default value
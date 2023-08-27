#pragma once

#include <glm/glm.hpp>

struct SceneInformation
{
    static glm::mat4 view;
    static glm::mat4 projection;
    static glm::vec4 camera_position_c;
    static glm::vec4 camera_view_vector;
    static glm::vec4 camera_up_vector;
};
#include "CursorRay.h"

// Raio projetado com o clique do mouse
CursorRay g_cursorRay;

CursorRay ComputeRayFromMouse(GLFWwindow* window, const glm::mat4& projMatrix, const glm::mat4& viewMatrix)
{
    // Pega a posição atual do cursor na janela.
    glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
    
    // Converte para normalized device coordinates
    // Canto inferior esquerdo -> (-1,-1) 
    // Canto superior direito -> (1,1)
    float x = (2.0f * g_LastCursorPosX) / g_actualWindowWidth - 1.0f;
    float y = 1.0f - (2.0f * g_LastCursorPosY) / g_actualWindowHeight;
    
    // Cria um vetor em espaço de recorte, com z e w definidos para representar a posição
    // na superfície do plano de corte próximo.
    glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

    // Transforma esse vetor para o espaço do olho (ou câmera) multiplicando-o
    // pela matriz de projeção invertida.
    glm::vec4 rayEye = glm::inverse(projMatrix) * rayClip;
    
    CursorRay ray;
    glm::vec4 rayStartPoint;
    glm::vec4 rayDir;

    // Se estiver usando projeção em perspectiva:
    if (g_UsePerspectiveProjection)
    { 
        // O ponto inicial do raio será a posição da câmera
        ray.startPoint = SceneInformation::camera_position_c;

        // Ajusta as coordenadas z e w do vetor rayEye
        rayEye.z = -1.0f;
        rayEye.w = 0.0f;

        // Transforma o vetor de direção do raio para coordenadas do mundo.
        glm::vec4 rayDirWorld = glm::inverse(viewMatrix) * rayEye;
        glm::vec4 rayDirection = glm::normalize(rayDirWorld);
        rayDirection.w = 0.0f;
        ray.direction = rayDirection;
    }
    else
    {
        // Transforma o vetor para coordenadas do mundo.
        glm::vec4 rayWorldPos = glm::inverse(viewMatrix) * rayEye;
        rayWorldPos.w = 1.0f;

        // O ponto de partida do raio será este ponto transformado.
        ray.startPoint = rayWorldPos;

        // Para projeção ortográfica, a direção do raio será o vetor de visão da câmera.
        ray.direction = glm::normalize(SceneInformation::camera_view_vector);
    }

    return ray;
}
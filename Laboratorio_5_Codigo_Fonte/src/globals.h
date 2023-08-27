#pragma once

#include <map>
#include <stack>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

extern float g_ScreenRatio;
extern float g_AngleX, g_AngleY, g_AngleZ;
extern bool g_LeftMouseButtonPressed, g_RightMouseButtonPressed, g_MiddleMouseButtonPressed;
extern bool tecla_W_pressionada, tecla_A_pressionada, tecla_S_pressionada, tecla_D_pressionada;
extern float g_CameraTheta, g_CameraPhi, g_CameraDistance;
extern float g_ForearmAngleZ, g_ForearmAngleX;
extern float g_TorsoPositionX, g_TorsoPositionY;
extern bool g_UsePerspectiveProjection;
extern bool g_ShowInfoText;

extern double g_LastCursorPosX, g_LastCursorPosY;

extern std::stack<glm::mat4> g_MatrixStack;
extern GLuint g_GpuProgramID;
extern GLint g_model_uniform;
extern GLint g_view_uniform;
extern GLint g_projection_uniform;
extern GLint g_object_id_uniform;
extern GLint g_bbox_min_uniform;
extern GLint g_bbox_max_uniform;
extern GLuint g_NumLoadedTextures;
extern int g_selectedObject;

extern int g_startWindowWidth, g_startWindowHeight;
extern float g_actualWindowWidth, g_actualWindowHeight;

// Map de instâncias para seu id
extern std::map<std::string, int> g_ObjectInstanceNameToIdMap;

// Paths dos modelos a serem carregados
extern std::vector<std::string> g_modelPaths;

// DEBUG
extern double g_glfwLastRayCursorPosX, g_glfwLastRayCursorPosY;
extern double g_NDCGlfwLastRayCursorPosX, g_NDCGlfwLastRayCursorPosY;
extern glm::vec4 g_rayClip, g_rayEye, g_rayWorld, g_rayDirection;

extern glm::vec4 g_rayStartPoint;
extern float g_rayLength;
extern glm::vec4 g_rayEndPoint;
extern glm::vec3 g_rayVertices[2];
extern GLuint VBO_ray_id;
extern GLuint VAO_ray_id;
extern std::string g_error;
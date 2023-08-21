#pragma once

#include <map>
#include <stack>
#include <string>
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
extern int selectedObject;
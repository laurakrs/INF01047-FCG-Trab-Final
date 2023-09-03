#pragma once

#include <map>
#include <stack>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

extern float g_ScreenRatio;
extern float g_AngleX, g_AngleY, g_AngleZ;
extern bool g_LeftMouseButtonPressed, g_LeftMouseButtonHold, g_LeftMouseButtonClicked, g_RightMouseButtonPressed, g_MiddleMouseButtonPressed;
extern float g_LeftMouseButtonPressTime;
extern bool w_key_pressed, a_key_pressed, s_key_pressed, d_key_pressed, q_key_pressed, e_key_pressed;
extern bool left_shift_key_pressed_repeat;
extern float g_CameraTheta, g_CameraPhi, g_CameraDistance;
extern float g_ForearmAngleZ, g_ForearmAngleX;
extern float g_TorsoPositionX, g_TorsoPositionY;
extern bool g_UsePerspectiveProjection;
extern bool g_ShowInfoText;

extern bool g_drawBoundingBox;
extern bool g_drawMouseRay;
extern bool g_pickAnimation;
extern double g_LastCursorPosX, g_LastCursorPosY;

extern std::stack<glm::mat4> g_MatrixStack;
extern GLuint g_GpuProgramID;
extern GLint g_model_uniform;
extern GLint g_view_uniform;
extern GLint g_projection_uniform;
extern GLint g_object_id_uniform;
extern GLint g_bbox_min_uniform;
extern GLint g_bbox_max_uniform;
extern GLint g_is_bounding_box_vertex_uniform;
extern GLint g_is_bounding_box_fragment_uniform;
extern GLuint g_NumLoadedTextures;

// Variáveis de seleção de objeto e animação
extern int g_selectedObject;
extern bool g_animateSelectedObject;
extern float g_animationStartTime;
extern glm::mat4 g_objectAnimationStartModelMatrix;
extern glm::vec4 g_selectedObjectCenter, g_selectedObjectScale, g_selectedObjectRotation;
extern std::string g_selectedObjectName;


extern int g_startWindowWidth, g_startWindowHeight;
extern float g_actualWindowWidth, g_actualWindowHeight;

// Objetos não selecionáveis:
#define CENTRAL_SPHERE 0
#define X_AXIS 1
#define Y_AXIS 2
#define Z_AXIS 3
// Objetos selecionáveis:
#define SPHERE 4
#define BUNNY  5
#define PLANE  6
#define COW    7
#define CUBE   8
#define RECTANGLE 9

// Paths de arquivos externos a serem carregados e seus ids
extern std::vector<std::tuple<std::string, int>> g_modelPathsAndIds;
extern std::vector<std::string> g_texturePaths;

// Variáveis para o raio
extern float g_rayLength;
extern GLuint VBO_ray_id;
extern GLuint VAO_ray_id;

// Variáveis do movimento da câmera
extern float g_current_time;
extern float g_delta_t;
extern float g_prev_time;
extern glm::vec4 g_vetor_w;
extern glm::vec4 g_vetor_u;

// DEBUG
extern float g_distSquared, g_radiusSquared, g_t1, g_t2, g_t;
extern glm::vec3 g_toSphere, g_closestPoint, g_centerToPoint;
extern bool g_isScaleEqualOnAllDirections;
extern glm::vec3 g_rayOrigin, g_rayDirection;
extern float g_dx, g_dy;
extern std::string g_printBoundingBox;
extern std::string g_debug_name, g_triangle_data;






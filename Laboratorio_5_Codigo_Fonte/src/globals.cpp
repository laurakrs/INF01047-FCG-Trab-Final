#include "globals.h"

// Razão de proporção da janela (largura/altura). Veja função FramebufferSizeCallback().
float g_ScreenRatio = 1.0f;

// Ângulos de Euler que controlam a rotação de um dos cubos da cena virtual
float g_AngleX = 0.0f;
float g_AngleY = 0.0f;
float g_AngleZ = 0.0f;

// "g_LeftMouseButtonPressed = true" se o usuário está com o botão esquerdo do mouse
// pressionado no momento atual. Veja função MouseButtonCallback().
bool g_LeftMouseButtonPressed = false;
bool g_RightMouseButtonPressed = false; // Análogo para botão direito do mouse
bool g_MiddleMouseButtonPressed = false; // Análogo para botão do meio do mouse

// Variaveis para as teclas que movimentam a camera. " = true" se pressionada
bool w_key_pressed = false;
bool a_key_pressed = false;
bool s_key_pressed = false;
bool d_key_pressed = false;

// Variáveis que definem a câmera em coordenadas esféricas, controladas pelo
// usuário através do mouse (veja função CursorPosCallback()). A posição
// efetiva da câmera é calculada dentro da função main(), dentro do loop de
// renderização.
float g_CameraTheta = 0.0f; // Ângulo no plano ZX em relação ao eixo Z
float g_CameraPhi = 0.0f;   // Ângulo em relação ao eixo Y
float g_CameraDistance = 3.5f; // Distância da câmera para a origem

// Variáveis que controlam rotação do antebraço
float g_ForearmAngleZ = 0.0f;
float g_ForearmAngleX = 0.0f;

// Variáveis que controlam translação do torso
float g_TorsoPositionX = 0.0f;
float g_TorsoPositionY = 0.0f;

// Variável que controla o tipo de projeção utilizada: perspectiva ou ortográfica.
bool g_UsePerspectiveProjection = true;

// Variável que controla se o texto informativo será mostrado na tela.
bool g_ShowInfoText = true;

// Variáveis globais que armazenam a última posição do cursor do mouse, para
// que possamos calcular quanto que o mouse se movimentou entre dois instantes
// de tempo. Utilizadas no callback CursorPosCallback() abaixo.
double g_LastCursorPosX, g_LastCursorPosY;
bool g_drawBoundingBox = true;
bool g_drawMouseRay = true;
bool g_pickAnimation = false;


// Pilha que guardará as matrizes de modelagem.
std::stack<glm::mat4>  g_MatrixStack;

// Variáveis que definem um programa de GPU (shaders). Veja função LoadShadersFromFiles().
GLuint g_GpuProgramID = 0;
GLint g_model_uniform;
GLint g_view_uniform;
GLint g_projection_uniform;
GLint g_object_id_uniform;
GLint g_bbox_min_uniform;
GLint g_bbox_max_uniform;
GLint g_is_bounding_box_vertex_uniform;
GLint g_is_bounding_box_fragment_uniform;

// Número de texturas carregadas pela função LoadTextureImage()
GLuint g_NumLoadedTextures = 0;

// Variáveis de seleção de objeto e animação
int g_selectedObject = -1;
bool g_animateSelectedObject = false;
float g_animationStartTime;
glm::mat4 g_objectAnimationStartModelMatrix;
glm::vec4 g_selectedObjectCenter, g_selectedObjectScale, g_selectedObjectRotation;
std::string g_selectedObjectName;

// Tamanho da janela (inicializado com valores padrões)
int g_startWindowWidth = 800;
int g_startWindowHeight = 600;

float g_actualWindowWidth;
float g_actualWindowHeight;

// Map de instâncias para seu id
std::map<std::string, int> g_ObjectInstanceNameToIdMap;

// Paths de arquivos externos a serem carregados
std::vector<std::string> g_modelPaths = 
{
    "../../data/sphere.obj",
    "../../data/bunny.obj",
    "../../data/plane.obj",
    "../../data/cow.obj",
    "../../data/cube.obj",
    "../../data/rectangle.obj"
};

std::vector<std::string> g_texturePaths = 
{
    "../../data/tc-earth_daymap_surface.jpg",
    "../../data/gravel_embedded_concrete_diff_4k.jpg",
    "../../data/wood_table_001_diff_4k.jpg",
    "../../data/fabric_leather_02_diff_4k.jpg",
    "../../data/leather_white_diff_4k.jpg"
};

// Variáveis para o raio
float g_rayLength = 1000.0f;
GLuint VBO_ray_id;
GLuint VAO_ray_id;

// DEBUG
float g_distSquared, g_radiusSquared, g_t1, g_t2, g_t;
glm::vec3 g_toSphere, g_closestPoint, g_centerToPoint;
bool g_isScaleEqualOnAllDirections;
glm::vec3 g_rayOrigin, g_rayDirection;







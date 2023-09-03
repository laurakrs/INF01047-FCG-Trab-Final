//     Universidade Federal do Rio Grande do Sul
//             Instituto de Informática
//       Departamento de Informática Aplicada
//
//    INF01047 Fundamentos de Computação Gráfica
//               Prof. Eduardo Gastal
//
//                   TRABALHO FINAL
//

// Arquivos "headers" padrões de C podem ser incluídos em um
// programa C++, sendo necessário somente adicionar o caractere
// "c" antes de seu nome, e remover o sufixo ".h". Exemplo:
//    #include <stdio.h> // Em C
//  vira
//    #include <cstdio> // Em C++
//
#include <cmath>
#include <cstdio>
#include <cstdlib>

// Headers abaixo são específicos de C++
#include <set>
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <vector>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers da biblioteca para carregar modelos obj
#include <tiny_obj_loader.h>

#include <stb_image.h>

// Headers locais, definidos na pasta "include/"
#include "utils.h"
#include "matrices.h"

// Headers de arquivos criados exclusivamente para o programa
#include "BoundingBox.h"
#include "ObjModel.h"
#include "Callbacks.h"
#include "textrendering.h"
#include "SceneObject.h"
#include "ObjectInstance.h"
#include "SceneInformation.h"
#include "CursorRay.h"
#include "GUI.h"
#include "collisions.h"

// Definição de constantes
#define M_PI   3.14159265358979323846

// Inicialização do ambiente
void InitializeGlfw();
GLFWwindow* InitializeWindow();

// Declaração de funções utilizadas para pilha de matrizes de modelagem.
void PushMatrix(glm::mat4 M);
void PopMatrix(glm::mat4& M);

// Funções callback para comunicação com o sistema operacional e interação do
// usuário. Veja mais comentários nas definições das mesmas, abaixo.
void SetCallbacks(GLFWwindow* window);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void UpdateMouseButtonStatus(GLFWwindow* window);

// Imprime informações do sistema
void PrintGPUInformation();

// Declaração de várias funções utilizadas em main().  Essas estão definidas
// logo após a definição de main() neste arquivo.
void LoadShadersFromFiles(); // Carrega os shaders de vértice e fragmento, criando um programa de GPU
GLuint LoadShader_Vertex(const char* filename);   // Carrega um vertex shader
GLuint LoadShader_Fragment(const char* filename); // Carrega um fragment shader
void LoadShader(const char* filename, GLuint shader_id); // Função utilizada pelas duas acima

// Geração dos objetos
void GenerateObjectModels(const std::vector<std::tuple<std::string, int>> g_modelPathsAndIds); // Constrói representações de objetos geométricos
void ComputeNormals(ObjModel* model); // Computa normais de um ObjModel, caso não existam.
void BuildTrianglesAndAddToVirtualScene(ObjModel* model, int sceneObjectId); // Constrói representação de um ObjModel como malha de triângulos para renderização
GLuint BuildTriangles(); // Constrói triângulos para renderização
void SetupBoundingBoxVAOAndVBO();
void LoadTextureImages(const std::vector<std::string>& texturePaths); // Carrega imagens de textura
void LoadTextureImage(const char* filename); // Função que carrega imagens de textura

// Criação de eixos da cena
void GenerateObjectInstances(glm::vec4 camera_lookat_l); // Constrói representações de instâncias de objetos geométricos
void SetupXYZAxesVAOAndVBO(GLuint &VAO_X_axis, GLuint &VAO_Y_axis, GLuint &VAO_Z_axis, GLuint &VBO_X_axis, GLuint &VBO_Y_axis, GLuint &VBO_Z_axis);
void SetupRayVAOAndVBO();

// Funções de desenho
void DrawVirtualObject(const char* object_name); // Desenha um objeto armazenado em g_VirtualScene
void DrawRay(glm::vec4 cameraPosition, glm::vec4 ray_direction);

// Funções de controle
void SetObjectInformationWindowData();

// Funções abaixo renderizam como texto na janela OpenGL algumas matrizes e
// outras informações do programa. Definidas após main().
void TextRendering_ShowModelViewProjection(GLFWwindow* window, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec4 p_model);



// CÓDIGO PRINCIPAL ===========================
int main(int argc, char* argv[])
{
    // Inicializamos o ambiente do GLFW
    InitializeGlfw();

    // Inicialização da janela do programa
    GLFWwindow* window = InitializeWindow();

    // Inicializamos a interface gráfica GUI que fica dentro da janela
    InitializeGUI(window);

    // Imprime informações do sistema no console
    PrintGPUInformation();

    // Carregamos os shaders de vértices e de fragmentos que serão utilizados
    // para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
    //
    LoadShadersFromFiles();

    // Carregamos imagens para serem utilizadas como textura
    LoadTextureImages(g_texturePaths);

    // Construímos a representação de objetos geométricos através de malhas de triângulos
    GenerateObjectModels(g_modelPathsAndIds);

    // Configuramos o VAO das bounding boxes dos elementos
    SetupBoundingBoxVAOAndVBO();

    // Modelo dos eixos XYZ
    GLuint VAO_X_axis, VAO_Y_axis, VAO_Z_axis;
    GLuint VBO_X_axis, VBO_Y_axis, VBO_Z_axis;
    SetupXYZAxesVAOAndVBO(VAO_X_axis, VAO_Y_axis, VAO_Z_axis, VBO_X_axis, VBO_Y_axis, VBO_Z_axis);

    // Modelo do raio a partir do clique do mouse
    SetupRayVAOAndVBO();
    
    if ( argc > 1 )
    {
        ObjModel model(argv[1]);
        BuildTrianglesAndAddToVirtualScene(&model, 0);
    }

    // Inicializamos o código para renderização de texto.
    TextRendering_Init();

    // Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
    glEnable(GL_DEPTH_TEST);

    // Habilitamos o Backface Culling. Veja slides 23-34 do documento Aula_13_Clipping_and_Culling.pdf e slides 112-123 do documento Aula_14_Laboratorio_3_Revisao.pdf.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    //  // VARIAVEIS ESPECIFICAS DA FREE CAMERA
    // Definição de propriedades da câmera
    float speed = 2.0f; // Velocidade da câmera
    float prev_time = (float)glfwGetTime();

    // Inicialização das coordenadas da free camera:
    float fixed_r = g_CameraDistance;
    float fixed_x = fixed_r*cos(g_CameraPhi)*sin(g_CameraTheta);
    float fixed_y = fixed_r*sin(g_CameraPhi);
    float fixed_z = fixed_r*cos(g_CameraPhi)*cos(g_CameraTheta);
    glm::vec4 camera_lookat_l = glm::vec4(0.0f,0.0f,0.0f,1.0f);

    // Geração das instâncias de objetos alterando a model matrix
    GenerateObjectInstances(camera_lookat_l);

    // Ficamos em um loop infinito, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    {
        // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor é
        // definida como coeficientes RGBA: Red, Green, Blue, Alpha; isto é:
        // Vermelho, Verde, Azul, Alpha (valor de transparência).
        // Conversaremos sobre sistemas de cores nas aulas de Modelos de Iluminação.
        //
        //           R     G     B     A
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        // "Pintamos" todos os pixels do framebuffer com a cor definida acima,
        // e também resetamos todos os pixels do Z-buffer (depth buffer).
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
        // os shaders de vértice e fragmentos).
        glUseProgram(g_GpuProgramID);

        // Computamos a posição da câmera utilizando coordenadas esféricas.  As
        // variáveis g_CameraDistance, g_CameraPhi, e g_CameraTheta são
        // controladas pelo mouse do usuário. Veja as funções CursorPosCallback()
        // e ScrollCallback().
        float r = g_CameraDistance;
        float y = r*sin(g_CameraPhi);
        float z = r*cos(g_CameraPhi)*cos(g_CameraTheta);
        float x = r*cos(g_CameraPhi)*sin(g_CameraTheta);

        bool isMovementKeyPressed = w_key_pressed || a_key_pressed || s_key_pressed || d_key_pressed || q_key_pressed || e_key_pressed;

        if (isMovementKeyPressed || g_RightMouseButtonPressed)
        {
            // Modo free camera
            SceneInformation::camera_position_c = glm::vec4(x,y,z,1.0f) + SceneInformation::camera_movement;
            camera_lookat_l = SceneInformation::origin + SceneInformation::camera_movement;
            x = SceneInformation::camera_position_c.x;
            y = SceneInformation::camera_position_c.y;
            z = SceneInformation::camera_position_c.z;
        }
        else
        {
            // Modo lookat camera
            SceneInformation::camera_position_c  = glm::vec4(x,y,z,1.0f) + SceneInformation::camera_movement; // Ponto "c", centro da câmera
            SceneInformation::camera_view_vector = camera_lookat_l - SceneInformation::camera_position_c; // Vetor "view", sentido para onde a câmera está virada
        }

        // FREE CAMERA
        // Definicoes da Free Camera
        g_vetor_w = -SceneInformation::camera_view_vector;
        g_vetor_u = crossproduct(SceneInformation::camera_up_vector, g_vetor_w);
        g_vetor_w = g_vetor_w / norm(g_vetor_w);
        g_vetor_u = g_vetor_u / norm(g_vetor_u);

        // Computamos a matriz "View" utilizando os parâmetros da câmera para
        // definir o sistema de coordenadas da câmera.  Veja slides 2-14, 184-190 e 236-242 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
        SceneInformation::view = Matrix_Camera_View(SceneInformation::camera_position_c, SceneInformation::camera_view_vector, SceneInformation::camera_up_vector);

        // Note que, no sistema de coordenadas da câmera, os planos near e far
        // estão no sentido negativo! Veja slides 176-204 do documento Aula_09_Projecoes.pdf.
        float nearplane = -0.1f;  // Posição do "near plane"
        float farplane  = -100.0f; // Posição do "far plane"

        if (g_UsePerspectiveProjection)
        {
            // Projeção Perspectiva.
            // Para definição do field of view (FOV), veja slides 205-215 do documento Aula_09_Projecoes.pdf.
            float field_of_view = 3.141592 / 3.0f;
            SceneInformation::projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);
        }
        else
        {
            // Projeção Ortográfica.
            // Para definição dos valores l, r, b, t ("left", "right", "bottom", "top"),
            // PARA PROJEÇÃO ORTOGRÁFICA veja slides 219-224 do documento Aula_09_Projecoes.pdf.
            // Para simular um "zoom" ortográfico, computamos o valor de "t"
            // utilizando a variável g_CameraDistance.
            float t = 1.5f*g_CameraDistance/2.5f;
            float b = -t;
            float r = t*g_ScreenRatio;
            float l = -r;
            SceneInformation::projection = Matrix_Orthographic(l, r, b, t, nearplane, farplane);
        }

        glm::mat4 model = Matrix_Identity(); // Transformação identidade de modelagem

        // Enviamos as matrizes "view" e "projection" para a placa de vídeo
        // (GPU). Veja o arquivo "shader_vertex.glsl", onde estas são
        // efetivamente aplicadas em todos os pontos.
        glUniformMatrix4fv(g_view_uniform       , 1 , GL_FALSE , glm::value_ptr(SceneInformation::view));
        glUniformMatrix4fv(g_projection_uniform , 1 , GL_FALSE , glm::value_ptr(SceneInformation::projection));

        // parece estar dando algum erro nesse loop quando debuga com
        // GLenum error = glGetError();
        // if(error != GL_NO_ERROR) {
        //     g_error += "Error glBindVertexArray: " + std::to_string(error) + "\n";
        // }
        // Gera as imagens dos objetos
        for (const auto& pair : g_ObjectInstances)
        {
            int instance_id = pair.first;
            ObjectInstance instance = pair.second;
            int sceneObjectId = instance.sceneObject_id;
            SceneObject sceneObject = g_idToSceneObject[sceneObjectId];

            // Modificação da posição da esfera
            if (sceneObjectId == CENTRAL_SPHERE)
            {
                instance.model_matrix = Matrix_Translate(camera_lookat_l.x,camera_lookat_l.y,camera_lookat_l.z)
                    * Matrix_Scale(0.05f,0.05f,0.05f);
            }

            glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(instance.model_matrix));
            glUniform1i(g_object_id_uniform, sceneObjectId);

            if (sceneObjectId == X_AXIS || sceneObjectId == Y_AXIS || sceneObjectId == Z_AXIS)
            {
                switch (sceneObjectId)
                {
                case X_AXIS:
                    glBindVertexArray(VAO_X_axis);
                    break;
                case Y_AXIS:
                    glBindVertexArray(VAO_Y_axis);
                    break;
                case Z_AXIS:
                    glBindVertexArray(VAO_Z_axis);
                    break;
                default:
                    break;
                }

                glDrawArrays(GL_LINES, 0, 2);  // 2 vertices for the X axis
                glBindVertexArray(0);
            }
            else
            {
                glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(instance.model_matrix));
                glUniform1i(g_object_id_uniform, sceneObjectId);
                DrawVirtualObject(sceneObject.name.c_str());
            }

        }
        
        if (g_LeftMouseButtonPressed)
        {
            g_cursorRay = ComputeRayFromMouse(window, SceneInformation::projection, SceneInformation::view);
            
            bool intersectsSomething = false;
            int nearest_object = -1;

            // Loop for each object in g_ObjectInstances
            for (const auto& pair : g_ObjectInstances)
            {
                int instance_id = pair.first;
                ObjectInstance instance = pair.second;
                std::string instance_name = instance.object_name;
                glm::mat4 model_matrix = instance.model_matrix;
                int sceneObjectId = instance.sceneObject_id;
                SceneObject sceneObject = g_idToSceneObject[sceneObjectId];
                std::string sceneObject_name = sceneObject.name;

                float intersection_distance;
                float nearest_intersection_distance = std::numeric_limits<float>::max();
                
                if (sceneObjectId == SPHERE)
                {
                    glm::vec4 sphere_bb_min = sceneObject.bbox_min;
                    glm::vec4 sphere_bb_max = sceneObject.bbox_max;
                    glm::vec4 sphere_bb_center = (sphere_bb_min + sphere_bb_max) / 2.0f;
                    float side_length = fabs(sphere_bb_max.x - sphere_bb_min.x);
                    float sphere_bb_radius = side_length / 2.0f;

                    intersectsSomething = RayIntersectsSphere(g_cursorRay.startPoint, g_cursorRay.direction, 
                                                            sphere_bb_center, sphere_bb_radius, 
                                                            model_matrix, intersection_distance);
                }
                else
                {
                    intersectsSomething = TestRayOBBIntersection(g_cursorRay.startPoint, g_cursorRay.direction, 
                                                             sceneObject.bbox_min, sceneObject.bbox_max, 
                                                             model_matrix, intersection_distance);
                }

                if (intersectsSomething)
                {
                    if (intersection_distance < nearest_intersection_distance)
                    {
                        nearest_object = instance_id;
                        nearest_intersection_distance = intersection_distance;
                    }
                }
            }


            if (nearest_object != -1)
            {
                g_selectedObject = nearest_object;
                g_animateSelectedObject = true;
                g_animationStartTime = (float)glfwGetTime();
                g_objectAnimationStartModelMatrix = g_ObjectInstances[g_selectedObject].model_matrix;
            }
            else
            {
                g_selectedObject = -1;
            }
            
        }

        // Atualiza as informações do objeto selecionado
        SetObjectInformationWindowData();

        UpdateMouseButtonStatus(window);

        // Desenha um raio a partir do mouse
        if (g_drawMouseRay)
        {
            DrawRay(g_cursorRay.startPoint, g_cursorRay.direction);
        }

        // Imprimimos na tela informação sobre o número de quadros renderizados
        // por segundo (frames per second).
        TextRendering_ShowFramesPerSecond(window);

        // Cálculo de delta logo antes do glfwSwapBuffers para tentar minimizar o atraso da geração de imagens
        // Atualiza delta de tempo
        g_current_time = (float)glfwGetTime();
        g_delta_t = g_current_time - g_prev_time;
        g_prev_time = g_current_time;

        // Realiza movimentação de objetos
        if (w_key_pressed)
            // Movimenta câmera para frente
            SceneInformation::camera_movement += -g_vetor_w * speed * g_delta_t;
        if (a_key_pressed)
            // Movimenta câmera para esquerda
            SceneInformation::camera_movement += -g_vetor_u * speed * g_delta_t;
        if (s_key_pressed)
            // Movimenta câmera para trás
            SceneInformation::camera_movement += g_vetor_w * speed * g_delta_t;
        if (d_key_pressed)
            // Movimenta câmera para direita
            SceneInformation::camera_movement += g_vetor_u * speed * g_delta_t;
        if (q_key_pressed)
            // Movimenta câmera para cima
            SceneInformation::camera_movement += SceneInformation::camera_up_vector * speed * g_delta_t;
        if (e_key_pressed)
            // Movimenta câmera para baixo
            SceneInformation::camera_movement += -SceneInformation::camera_up_vector * speed * g_delta_t;

        if (g_animateSelectedObject && g_pickAnimation)
        {
            float elapsed_time = g_current_time - g_animationStartTime;

            glm::mat4 current_model_matrix = g_ObjectInstances[g_selectedObject].model_matrix * Matrix_Translate(0.0f,1.0f,0.0f);
            if (elapsed_time < 0.5f)
            {
                g_ObjectInstances[g_selectedObject].model_matrix = g_ObjectInstances[g_selectedObject].model_matrix * Matrix_Translate(0.0f,g_delta_t,0.0f);
            }
            else if (elapsed_time < 1.0f)
            {
                g_ObjectInstances[g_selectedObject].model_matrix = g_ObjectInstances[g_selectedObject].model_matrix * Matrix_Rotate_Y(g_delta_t * M_PI * 4);
            }
            else if (elapsed_time < 1.5f)
            {
                g_ObjectInstances[g_selectedObject].model_matrix = g_ObjectInstances[g_selectedObject].model_matrix * Matrix_Translate(0.0f,-g_delta_t,0.0f);
            }
            else if (elapsed_time < 1.8f)
            {
                g_ObjectInstances[g_selectedObject].model_matrix = g_objectAnimationStartModelMatrix;
                g_animateSelectedObject = false;
            }
        }

        GenerateGUIWindows();

        // O framebuffer onde OpenGL executa as operações de renderização não
        // é o mesmo que está sendo mostrado para o usuário, caso contrário
        // seria possível ver artefatos conhecidos como "screen tearing". A
        // chamada abaixo faz a troca dos buffers, mostrando para o usuário
        // tudo que foi renderizado pelas funções acima.
        // Veja o link: https://en.wikipedia.org/w/index.php?title=Multiple_buffering&oldid=793452829#Double_buffering_in_computer_graphics
        glfwSwapBuffers(window);

        // Verificamos com o sistema operacional se houve alguma interação do
        // usuário (teclado, mouse, ...). Caso positivo, as funções de callback
        // definidas anteriormente usando glfwSet*Callback() serão chamadas
        // pela biblioteca GLFW.
        glfwPollEvents();
    }

    // Finalizamos o uso dos recursos da interface gráfica IMGUI
    DisposeGUI();

    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    // Fim do programa
    return 0;
}



// Inicialização do ambiente ======================================================================================================
GLFWwindow* InitializeWindow()
{
    // Criamos uma janela do sistema operacional, com 800 colunas e 600 linhas
    // de pixels, e com título "INF01047 ...".
    GLFWwindow* window;
    window = glfwCreateWindow(g_startWindowWidth, g_startWindowHeight, "INF01047 - Trabalho Final - Laura Keidann e Matheus Sabadin", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Seta KeyCallback, MouseButtonCallback, CursorPosCallback e ScrollCallback
    SetCallbacks(window);

    // Indicamos que as chamadas OpenGL deverão renderizar nesta janela
    glfwMakeContextCurrent(window);

    // Carregamento de todas funções definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // Definimos a função de callback que será chamada sempre que a janela for
    // redimensionada, por consequência alterando o tamanho do "framebuffer"
    // (região de memória onde são armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    FramebufferSizeCallback(window, g_startWindowWidth, g_startWindowHeight); // Forçamos a chamada do callback acima, para definir g_ScreenRatio.

    return window;
}

void InitializeGlfw()
{
    // Inicializamos a biblioteca GLFW, utilizada para criar uma janela do
    // sistema operacional, onde poderemos renderizar com OpenGL.
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos o callback para impressão de erros da GLFW no terminal
    glfwSetErrorCallback(ErrorCallback);

    // Pedimos para utilizar OpenGL versão 3.3 (ou superior)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Pedimos para utilizar o perfil "core", isto é, utilizaremos somente as
    // funções modernas de OpenGL.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}



// Definição dos callbacks ======================================================================================================
void SetCallbacks(GLFWwindow* window)
{
       // Definimos a função de callback que será chamada sempre que o usuário
    // pressionar alguma tecla do teclado ...
    glfwSetKeyCallback(window, KeyCallback);
    // ... ou clicar os botões do mouse ...
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    // ... ou movimentar o cursor do mouse em cima da janela ...
    glfwSetCursorPosCallback(window, CursorPosCallback);
    // ... ou rolar a "rodinha" do mouse.
    glfwSetScrollCallback(window, ScrollCallback);
}

// Definição da função que será chamada sempre que o usuário pressionar alguma
// tecla do teclado. Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    // ==================
    // Não modifique este loop! Ele é utilizando para correção automatizada dos
    // laboratórios. Deve ser sempre o primeiro comando desta função KeyCallback().
    for (int i = 0; i < 10; ++i)
        if (key == GLFW_KEY_0 + i && action == GLFW_PRESS && mod == GLFW_MOD_SHIFT)
            std::exit(100 + i);
    // ==================

    // Se o usuário pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // O código abaixo implementa a seguinte lógica:
    //   Se apertar tecla X       então g_AngleX += delta;
    //   Se apertar tecla shift+X então g_AngleX -= delta;
    //   Se apertar tecla Y       então g_AngleY += delta;
    //   Se apertar tecla shift+Y então g_AngleY -= delta;
    //   Se apertar tecla Z       então g_AngleZ += delta;
    //   Se apertar tecla shift+Z então g_AngleZ -= delta;

    float delta = 3.141592 / 16; // 22.5 graus, em radianos.

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        g_AngleX += (mod & GLFW_MOD_SHIFT) ? -delta : delta;
    }

    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
    {
        g_AngleY += (mod & GLFW_MOD_SHIFT) ? -delta : delta;
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        g_AngleZ += (mod & GLFW_MOD_SHIFT) ? -delta : delta;
    }

    // Se o usuário apertar a tecla espaço, resetamos os ângulos de Euler para zero.
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        g_AngleX = 0.0f;
        g_AngleY = 0.0f;
        g_AngleZ = 0.0f;
        g_ForearmAngleX = 0.0f;
        g_ForearmAngleZ = 0.0f;
        g_TorsoPositionX = 0.0f;
        g_TorsoPositionY = 0.0f;
    }

    // Se o usuário apertar a tecla H, fazemos um "toggle" do texto informativo mostrado na tela.
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        g_ShowInfoText = !g_ShowInfoText;
    }

    // Se o usuário apertar a tecla R, recarregamos os shaders dos arquivos "shader_fragment.glsl" e "shader_vertex.glsl".
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        LoadShadersFromFiles();
        fprintf(stdout,"Shaders recarregados!\n");
        fflush(stdout);
    }

    // Tarefa 4 - LAB 2 0 FREE CAMERA
    // A tecla 'W' deve movimentar a câmera para FRENTE (em relação ao sistema de coordenadas da câmera);
    // A tecla 'S' deve movimentar a câmera para TRÁS (em relação ao sistema de coordenadas da câmera);
    // A tecla 'D' deve movimentar a câmera para DIREITA (em relação ao sistema de coordenadas da câmera);
    // A tecla 'A' deve movimentar a câmera para ESQUERDA (em relação ao sistema de coordenadas da câmera);

    if (key == GLFW_KEY_W)
    {
        if (action == GLFW_PRESS)
            // Usuário apertou a tecla W, então atualizamos o estado para pressionada
            w_key_pressed = true;
        else if (action == GLFW_RELEASE)
            // Usuário largou a tecla W, então atualizamos o estado para NÃO pressionada
            w_key_pressed = false;
        else if (action == GLFW_REPEAT)
            // Usuário está segurando a tecla W e o sistema operacional está
            // disparando eventos de repetição. Neste caso, não precisamos
            // atualizar o estado da tecla, pois antes de um evento REPEAT
            // necessariamente deve ter ocorrido um evento PRESS.
            ;
    }
    if (key == GLFW_KEY_A)
    {
        if (action == GLFW_PRESS)
            // Usuário apertou a tecla A, então atualizamos o estado para pressionada
            a_key_pressed = true;
        else if (action == GLFW_RELEASE)
            // Usuário largou a tecla A, então atualizamos o estado para NÃO pressionada
            a_key_pressed = false;
        else if (action == GLFW_REPEAT)
            // Usuário está segurando a tecla A e o sistema operacional está
            // disparando eventos de repetição. Neste caso, não precisamos
            // atualizar o estado da tecla, pois antes de um evento REPEAT
            // necessariamente deve ter ocorrido um evento PRESS.
            ;
    }
    if (key == GLFW_KEY_S)
    {
        if (action == GLFW_PRESS)
            // Usuário apertou a tecla S, então atualizamos o estado para pressionada
            s_key_pressed = true;
        else if (action == GLFW_RELEASE)
            // Usuário largou a tecla S, então atualizamos o estado para NÃO pressionada
            s_key_pressed = false;
        else if (action == GLFW_REPEAT)
            // Usuário está segurando a tecla S e o sistema operacional está
            // disparando eventos de repetição. Neste caso, não precisamos
            // atualizar o estado da tecla, pois antes de um evento REPEAT
            // necessariamente deve ter ocorrido um evento PRESS.
            ;
    }
    if (key == GLFW_KEY_D)
    {
        if (action == GLFW_PRESS)
            // Usuário apertou a tecla D, então atualizamos o estado para pressionada
            d_key_pressed = true;
        else if (action == GLFW_RELEASE)
            // Usuário largou a tecla D, então atualizamos o estado para NÃO pressionada
            d_key_pressed = false;
        else if (action == GLFW_REPEAT)
            // Usuário está segurando a tecla D e o sistema operacional está
            // disparando eventos de repetição. Neste caso, não precisamos
            // atualizar o estado da tecla, pois antes de um evento REPEAT
            // necessariamente deve ter ocorrido um evento PRESS.
            ;
    }
    if (key == GLFW_KEY_Q)
    {
        if (action == GLFW_PRESS)
            q_key_pressed = true;
        else if (action == GLFW_RELEASE)
            q_key_pressed = false;
        else if (action == GLFW_REPEAT)
            ;
    }
    if (key == GLFW_KEY_E)
    {
        if (action == GLFW_PRESS)
            e_key_pressed = true;
        else if (action == GLFW_RELEASE)
            e_key_pressed = false;
        else if (action == GLFW_REPEAT)
            ;
    }
    


    if (g_selectedObject != -1)
    {
        float translation_x = 0.0f;
        float translation_y = 0.0f;
        float translation_z = 0.0f;

        switch (key)
        {
        case GLFW_KEY_DOWN:
            translation_z = 1.1f;
            break;
        case GLFW_KEY_UP:
            translation_z = -1.1f;
            break;
        case GLFW_KEY_RIGHT:
            translation_x = 1.1f;
            break;
        case GLFW_KEY_LEFT:
            translation_x = -1.1f;
            break;
        default:
            break;
        }

        if (action == GLFW_PRESS)
        {
            g_ObjectInstances[g_selectedObject].model_matrix = g_ObjectInstances[g_selectedObject].model_matrix * Matrix_Translate(translation_x,translation_y,translation_z);
        }
        else if (action == GLFW_RELEASE)
            ;
        else if (action == GLFW_REPEAT)
        {
            g_ObjectInstances[g_selectedObject].model_matrix = g_ObjectInstances[g_selectedObject].model_matrix * Matrix_Translate(translation_x,translation_y,translation_z);
        }
    }
}

// Função callback chamada sempre que o usuário movimentar o cursor do mouse em
// cima da janela OpenGL.
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (g_LeftMouseButtonHold && g_selectedObject != -1)
    {
        // Deslocamento do cursor do mouse em x e y de coordenadas de tela!
        float dx = xpos - g_LastCursorPosX;
        float dy = ypos - g_LastCursorPosY;

        // change the model matrix according to the mouse movement
        g_ObjectInstances[g_selectedObject].model_matrix = g_ObjectInstances[g_selectedObject].model_matrix
        * Matrix_Translate(dx * 0.01f, 0.0f, dy * 0.01f);

        // Atualizamos as variáveis globais para armazenar a posição atual do
        // cursor como sendo a última posição conhecida do cursor.
        g_LastCursorPosX = xpos;
        g_LastCursorPosY = ypos;
    }

    // Para o pan da câmera
    if (g_RightMouseButtonPressed)
    {
        // Deslocamento do cursor do mouse em x e y de coordenadas de tela!
        float dx = xpos - g_LastCursorPosX;
        float dy = ypos - g_LastCursorPosY;

        // Movimentação da câmera com o botão direito do mouse
        SceneInformation::camera_movement += g_vetor_u * dx * g_delta_t;
        SceneInformation::camera_movement += -SceneInformation::camera_up_vector * dy * g_delta_t;
    
        // Atualizamos as variáveis globais para armazenar a posição atual do
        // cursor como sendo a última posição conhecida do cursor.
        g_LastCursorPosX = xpos;
        g_LastCursorPosY = ypos;
    }

    // Para o giro da câmera
    if (g_MiddleMouseButtonPressed)
    {
        // Deslocamento do cursor do mouse em x e y de coordenadas de tela!
        float dx = xpos - g_LastCursorPosX;
        float dy = ypos - g_LastCursorPosY;

        // Atualizamos parâmetros da câmera com os deslocamentos
        g_CameraTheta -= 0.01f*dx;
        g_CameraPhi   += 0.01f*dy;
    
        // Em coordenadas esféricas, o ângulo phi deve ficar entre -pi/2 e +pi/2.
        float phimax = 3.141592f/2;
        float phimin = -phimax;
    
        if (g_CameraPhi > phimax)
            g_CameraPhi = phimax;
    
        if (g_CameraPhi < phimin)
            g_CameraPhi = phimin;
    
        // Atualizamos as variáveis globais para armazenar a posição atual do
        // cursor como sendo a última posição conhecida do cursor.
        g_LastCursorPosX = xpos;
        g_LastCursorPosY = ypos;
    }
}

// Função para verificar se o botão esquerdo do mouse teve apenas um clique ou está pressionado
void UpdateMouseButtonStatus(GLFWwindow* window)
{
    if (g_LeftMouseButtonPressed && !g_LeftMouseButtonHold)
    {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (glfwGetTime() - g_LeftMouseButtonPressTime > 0.05f))
        {
            g_LeftMouseButtonHold = true;
            g_LeftMouseButtonPressed = false;
        }
    }
}

// Impressão de informações ======================================================================================================
void PrintGPUInformation()
{
    // Imprimimos no terminal informações sobre a GPU do sistema
    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);
}



// Operações com shaders ======================================================================================================
// Função que carrega os shaders de vértices e de fragmentos que serão
// utilizados para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
//
void LoadShadersFromFiles()
{
    // Note que o caminho para os arquivos "shader_vertex.glsl" e
    // "shader_fragment.glsl" estão fixados, sendo que assumimos a existência
    // da seguinte estrutura no sistema de arquivos:
    //
    //    + FCG_Lab_01/
    //    |
    //    +--+ bin/
    //    |  |
    //    |  +--+ Release/  (ou Debug/ ou Linux/)
    //    |     |
    //    |     o-- main.exe
    //    |
    //    +--+ src/
    //       |
    //       o-- shader_vertex.glsl
    //       |
    //       o-- shader_fragment.glsl
    //
    GLuint vertex_shader_id = LoadShader_Vertex("../../src/shader_vertex.glsl");
    GLuint fragment_shader_id = LoadShader_Fragment("../../src/shader_fragment.glsl");

    // Deletamos o programa de GPU anterior, caso ele exista.
    if ( g_GpuProgramID != 0 )
        glDeleteProgram(g_GpuProgramID);

    // Criamos um programa de GPU utilizando os shaders carregados acima.
    g_GpuProgramID = CreateGpuProgram(vertex_shader_id, fragment_shader_id);

    // Buscamos o endereço das variáveis definidas dentro do Vertex Shader.
    // Utilizaremos estas variáveis para enviar dados para a placa de vídeo
    // (GPU)! Veja arquivo "shader_vertex.glsl" e "shader_fragment.glsl".
    g_model_uniform      = glGetUniformLocation(g_GpuProgramID, "model"); // Variável da matriz "model"
    g_view_uniform       = glGetUniformLocation(g_GpuProgramID, "view"); // Variável da matriz "view" em shader_vertex.glsl
    g_projection_uniform = glGetUniformLocation(g_GpuProgramID, "projection"); // Variável da matriz "projection" em shader_vertex.glsl
    g_object_id_uniform  = glGetUniformLocation(g_GpuProgramID, "object_id"); // Variável "object_id" em shader_fragment.glsl
    g_bbox_min_uniform   = glGetUniformLocation(g_GpuProgramID, "bbox_min");
    g_bbox_max_uniform   = glGetUniformLocation(g_GpuProgramID, "bbox_max");

    g_is_bounding_box_vertex_uniform = glGetUniformLocation(g_GpuProgramID, "isBoundingBoxVertex");
    g_is_bounding_box_fragment_uniform = glGetUniformLocation(g_GpuProgramID, "isBoundingBoxFragment");

    // Variáveis em "shader_fragment.glsl" para acesso das imagens de textura
    glUseProgram(g_GpuProgramID);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImage0"), 0);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImage1"), 1);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImage2"), 2);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImage3"), 3);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImage4"), 4);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureImage5"), 5);
    glUseProgram(0);
}

// Função que pega a matriz M e guarda a mesma no topo da pilha
void PushMatrix(glm::mat4 M)
{
    g_MatrixStack.push(M);
}

// Função que remove a matriz atualmente no topo da pilha e armazena a mesma na variável M
void PopMatrix(glm::mat4& M)
{
    if ( g_MatrixStack.empty() )
    {
        M = Matrix_Identity();
    }
    else
    {
        M = g_MatrixStack.top();
        g_MatrixStack.pop();
    }
}

// Carrega um Vertex Shader de um arquivo GLSL. Veja definição de LoadShader() abaixo.
GLuint LoadShader_Vertex(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos vértices.
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, vertex_shader_id);

    // Retorna o ID gerado acima
    return vertex_shader_id;
}

// Carrega um Fragment Shader de um arquivo GLSL . Veja definição de LoadShader() abaixo.
GLuint LoadShader_Fragment(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos fragmentos.
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, fragment_shader_id);

    // Retorna o ID gerado acima
    return fragment_shader_id;
}

// Função auxilar, utilizada pelas duas funções acima. Carrega código de GPU de
// um arquivo GLSL e faz sua compilação.
void LoadShader(const char* filename, GLuint shader_id)
{
    // Lemos o arquivo de texto indicado pela variável "filename"
    // e colocamos seu conteúdo em memória, apontado pela variável
    // "shader_string".
    std::ifstream file;
    try {
        file.exceptions(std::ifstream::failbit);
        file.open(filename);
    } catch ( std::exception& e ) {
        fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }
    std::stringstream shader;
    shader << file.rdbuf();
    std::string str = shader.str();
    const GLchar* shader_string = str.c_str();
    const GLint   shader_string_length = static_cast<GLint>( str.length() );

    // Define o código do shader GLSL, contido na string "shader_string"
    glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

    // Compila o código do shader GLSL (em tempo de execução)
    glCompileShader(shader_id);

    // Verificamos se ocorreu algum erro ou "warning" durante a compilação
    GLint compiled_ok;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

    GLint log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    // Alocamos memória para guardar o log de compilação.
    // A chamada "new" em C++ é equivalente ao "malloc()" do C.
    GLchar* log = new GLchar[log_length];
    glGetShaderInfoLog(shader_id, log_length, &log_length, log);

    // Imprime no terminal qualquer erro ou "warning" de compilação
    if ( log_length != 0 )
    {
        std::string  output;

        if ( !compiled_ok )
        {
            output += "ERROR: OpenGL compilation of \"";
            output += filename;
            output += "\" failed.\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }
        else
        {
            output += "WARNING: OpenGL compilation of \"";
            output += filename;
            output += "\".\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }

        fprintf(stderr, "%s", output.c_str());
    }

    // A chamada "delete" em C++ é equivalente ao "free()" do C
    delete [] log;
}




// Geração dos modelos ======================================================================================================
void GenerateObjectModels(const std::vector<std::tuple<std::string, int>> g_modelPathsAndIds)
{
    for (const auto& pathAndId : g_modelPathsAndIds)
    {
        std::string path = std::get<0>(pathAndId);
        int id = std::get<1>(pathAndId);
        ObjModel model(path.c_str());
        ComputeNormals(&model);
        BuildTrianglesAndAddToVirtualScene(&model, id);
    }
}

// Função que computa as normais de um ObjModel, caso elas não tenham sido
// especificadas dentro do arquivo ".obj"
void ComputeNormals(ObjModel* model)
{
    if ( !model->attrib.normals.empty() )
        return;

    // Primeiro computamos as normais para todos os TRIÂNGULOS.
    // Segundo, computamos as normais dos VÉRTICES através do método proposto
    // por Gouraud, onde a normal de cada vértice vai ser a média das normais de
    // todas as faces que compartilham este vértice e que pertencem ao mesmo "smoothing group".

    // Obtemos a lista dos smoothing groups que existem no objeto
    std::set<unsigned int> sgroup_ids;
    for (size_t shape = 0; shape < model->shapes.size(); ++shape)
    {
        size_t num_triangles = model->shapes[shape].mesh.num_face_vertices.size();

        assert(model->shapes[shape].mesh.smoothing_group_ids.size() == num_triangles);

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(model->shapes[shape].mesh.num_face_vertices[triangle] == 3);
            unsigned int sgroup = model->shapes[shape].mesh.smoothing_group_ids[triangle];
            assert(sgroup >= 0);
            sgroup_ids.insert(sgroup);
        }
    }

    size_t num_vertices = model->attrib.vertices.size() / 3;
    model->attrib.normals.reserve( 3*num_vertices );

    // Processamos um smoothing group por vez
    for (const unsigned int & sgroup : sgroup_ids)
    {
        std::vector<int> num_triangles_per_vertex(num_vertices, 0);
        std::vector<glm::vec4> vertex_normals(num_vertices, glm::vec4(0.0f,0.0f,0.0f,0.0f));

        // Acumulamos as normais dos vértices de todos triângulos deste smoothing group
        for (size_t shape = 0; shape < model->shapes.size(); ++shape)
        {
            size_t num_triangles = model->shapes[shape].mesh.num_face_vertices.size();

            for (size_t triangle = 0; triangle < num_triangles; ++triangle)
            {
                unsigned int sgroup_tri = model->shapes[shape].mesh.smoothing_group_ids[triangle];

                if (sgroup_tri != sgroup)
                    continue;

                glm::vec4  vertices[3];
                for (size_t vertex = 0; vertex < 3; ++vertex)
                {
                    tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];
                    const float vx = model->attrib.vertices[3*idx.vertex_index + 0];
                    const float vy = model->attrib.vertices[3*idx.vertex_index + 1];
                    const float vz = model->attrib.vertices[3*idx.vertex_index + 2];
                    vertices[vertex] = glm::vec4(vx,vy,vz,1.0);
                }

                const glm::vec4  a = vertices[0];
                const glm::vec4  b = vertices[1];
                const glm::vec4  c = vertices[2];

                const glm::vec4  n = crossproduct(b-a,c-a);

                for (size_t vertex = 0; vertex < 3; ++vertex)
                {
                    tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];
                    num_triangles_per_vertex[idx.vertex_index] += 1;
                    vertex_normals[idx.vertex_index] += n;
                }
            }
        }

        // Computamos a média das normais acumuladas
        std::vector<size_t> normal_indices(num_vertices, 0);

        for (size_t vertex_index = 0; vertex_index < vertex_normals.size(); ++vertex_index)
        {
            if (num_triangles_per_vertex[vertex_index] == 0)
                continue;

            glm::vec4 n = vertex_normals[vertex_index] / (float)num_triangles_per_vertex[vertex_index];
            n /= norm(n);

            model->attrib.normals.push_back( n.x );
            model->attrib.normals.push_back( n.y );
            model->attrib.normals.push_back( n.z );

            size_t normal_index = (model->attrib.normals.size() / 3) - 1;
            normal_indices[vertex_index] = normal_index;
        }

        // Escrevemos os índices das normais para os vértices dos triângulos deste smoothing group
        for (size_t shape = 0; shape < model->shapes.size(); ++shape)
        {
            size_t num_triangles = model->shapes[shape].mesh.num_face_vertices.size();

            for (size_t triangle = 0; triangle < num_triangles; ++triangle)
            {
                unsigned int sgroup_tri = model->shapes[shape].mesh.smoothing_group_ids[triangle];

                if (sgroup_tri != sgroup)
                    continue;

                for (size_t vertex = 0; vertex < 3; ++vertex)
                {
                    tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];
                    model->shapes[shape].mesh.indices[3*triangle + vertex].normal_index =
                        normal_indices[ idx.vertex_index ];
                }
            }
        }

    }
}

// Constrói triângulos para futura renderização a partir de um ObjModel.
void BuildTrianglesAndAddToVirtualScene(ObjModel* model, int sceneObjectId)
{
    // Criamos um VAO para o objeto
    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);
    glBindVertexArray(vertex_array_object_id);

    std::vector<GLuint> indices;
    std::vector<float>  model_coefficients;
    std::vector<float>  normal_coefficients;
    std::vector<float>  texture_coefficients;

    for (size_t shape = 0; shape < model->shapes.size(); ++shape)
    {
        size_t first_index = indices.size();
        size_t num_triangles = model->shapes[shape].mesh.num_face_vertices.size();

        const float minval = std::numeric_limits<float>::min();
        const float maxval = std::numeric_limits<float>::max();

        glm::vec4 bbox_min = glm::vec4(maxval,maxval,maxval,1.0f);
        glm::vec4 bbox_max = glm::vec4(minval,minval,minval,1.0f);

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(model->shapes[shape].mesh.num_face_vertices[triangle] == 3);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];

                indices.push_back(first_index + 3*triangle + vertex);

                const float vx = model->attrib.vertices[3*idx.vertex_index + 0];
                const float vy = model->attrib.vertices[3*idx.vertex_index + 1];
                const float vz = model->attrib.vertices[3*idx.vertex_index + 2];
                //printf("tri %d vert %d = (%.2f, %.2f, %.2f)\n", (int)triangle, (int)vertex, vx, vy, vz);
                model_coefficients.push_back( vx ); // X
                model_coefficients.push_back( vy ); // Y
                model_coefficients.push_back( vz ); // Z
                model_coefficients.push_back( 1.0f ); // W

                bbox_min.x = std::min(bbox_min.x, vx);
                bbox_min.y = std::min(bbox_min.y, vy);
                bbox_min.z = std::min(bbox_min.z, vz);
                bbox_max.x = std::max(bbox_max.x, vx);
                bbox_max.y = std::max(bbox_max.y, vy);
                bbox_max.z = std::max(bbox_max.z, vz);

                // Inspecionando o código da tinyobjloader, o aluno Bernardo
                // Sulzbach (2017/1) apontou que a maneira correta de testar se
                // existem normais e coordenadas de textura no ObjModel é
                // comparando se o índice retornado é -1. Fazemos isso abaixo.

                if ( idx.normal_index != -1 )
                {
                    const float nx = model->attrib.normals[3*idx.normal_index + 0];
                    const float ny = model->attrib.normals[3*idx.normal_index + 1];
                    const float nz = model->attrib.normals[3*idx.normal_index + 2];
                    normal_coefficients.push_back( nx ); // X
                    normal_coefficients.push_back( ny ); // Y
                    normal_coefficients.push_back( nz ); // Z
                    normal_coefficients.push_back( 0.0f ); // W
                }

                if ( idx.texcoord_index != -1 )
                {
                    const float u = model->attrib.texcoords[2*idx.texcoord_index + 0];
                    const float v = model->attrib.texcoords[2*idx.texcoord_index + 1];
                    texture_coefficients.push_back( u );
                    texture_coefficients.push_back( v );
                }
            }
        }

        size_t last_index = indices.size() - 1;

        SceneObject theobject;
        theobject.name           = model->shapes[shape].name;
        theobject.first_index    = first_index; // Primeiro índice
        theobject.num_indices    = last_index - first_index + 1; // Número de indices
        theobject.rendering_mode = GL_TRIANGLES;       // Índices correspondem ao tipo de rasterização GL_TRIANGLES.
        theobject.vertex_array_object_id = vertex_array_object_id;

        theobject.bbox_min = bbox_min;
        theobject.bbox_max = bbox_max;


        // ======================
        // Adição da bounding box

        glm::vec4 v[8];
        v[0] = glm::vec4(bbox_min.x, bbox_min.y, bbox_min.z, 1.0f);
        v[1] = glm::vec4(bbox_max.x, bbox_min.y, bbox_min.z, 1.0f);
        v[2] = glm::vec4(bbox_max.x, bbox_max.y, bbox_min.z, 1.0f);
        v[3] = glm::vec4(bbox_min.x, bbox_max.y, bbox_min.z, 1.0f);
        v[4] = glm::vec4(bbox_min.x, bbox_min.y, bbox_max.z, 1.0f);
        v[5] = glm::vec4(bbox_max.x, bbox_min.y, bbox_max.z, 1.0f);
        v[6] = glm::vec4(bbox_max.x, bbox_max.y, bbox_max.z, 1.0f);
        v[7] = glm::vec4(bbox_min.x, bbox_max.y, bbox_max.z, 1.0f);

        for (int i = 0; i < 8; i++) 
        {
            theobject.bbox_vertices[i] = v[i];
        }
        // ======================

        std::string object_name = model->shapes[shape].name;
        g_VirtualScene[object_name] = theobject;
        g_idToSceneObject[sceneObjectId] = theobject;
    }

    GLuint VBO_model_coefficients_id;
    glGenBuffers(1, &VBO_model_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
    GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
    GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if ( !normal_coefficients.empty() )
    {
        GLuint VBO_normal_coefficients_id;
        glGenBuffers(1, &VBO_normal_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER, normal_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, normal_coefficients.size() * sizeof(float), normal_coefficients.data());
        location = 1; // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if ( !texture_coefficients.empty() )
    {
        GLuint VBO_texture_coefficients_id;
        glGenBuffers(1, &VBO_texture_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER, texture_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, texture_coefficients.size() * sizeof(float), texture_coefficients.data());
        location = 2; // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 2; // vec2 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    // "Ligamos" o buffer. Note que o tipo agora é GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);
}

// Inicializa o VAO e o VBO das bounding boxes dos elementos
void SetupBoundingBoxVAOAndVBO() 
{
    for (auto& pair : g_VirtualScene) {
        SceneObject &object = pair.second;
        
        // VAO for the bounding box
        GLuint bbox_vertex_array_object_id;
        glGenVertexArrays(1, &bbox_vertex_array_object_id);
        glBindVertexArray(bbox_vertex_array_object_id);

        // Define line pairs for the bounding box
        std::vector<glm::vec4> lines = {
            object.bbox_vertices[0], object.bbox_vertices[1],
            object.bbox_vertices[1], object.bbox_vertices[2],
            object.bbox_vertices[2], object.bbox_vertices[3],
            object.bbox_vertices[3], object.bbox_vertices[0],
            object.bbox_vertices[4], object.bbox_vertices[5],
            object.bbox_vertices[5], object.bbox_vertices[6],
            object.bbox_vertices[6], object.bbox_vertices[7],
            object.bbox_vertices[7], object.bbox_vertices[4],
            object.bbox_vertices[0], object.bbox_vertices[4],
            object.bbox_vertices[1], object.bbox_vertices[5],
            object.bbox_vertices[2], object.bbox_vertices[6],
            object.bbox_vertices[3], object.bbox_vertices[7]
        };

        GLuint VBO_bbox_coefficients_id;
        glGenBuffers(1, &VBO_bbox_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_bbox_coefficients_id);

        // Send the `lines` data to GPU
        glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(glm::vec4), lines.data(), GL_STATIC_DRAW);
        
        GLuint location = 0; // Assuming location 3 for bbox vertices in shader
        GLint number_of_dimensions = 4; // vec4
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);

        // Assign the newly created VAO to the object
        object.bbox_vertex_array_object_id = bbox_vertex_array_object_id;

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

// Carrega texturas de arquivos externos
void LoadTextureImages(const std::vector<std::string>& texturePaths)
{
    for(const auto& path : texturePaths)
    {
        LoadTextureImage(path.c_str());
    }
}

// Função que carrega uma imagem para ser utilizada como textura
void LoadTextureImage(const char* filename)
{
    printf("Carregando imagem \"%s\"... ", filename);

    // Primeiro fazemos a leitura da imagem do disco
    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    int channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 3);

    if ( data == NULL )
    {
        fprintf(stderr, "ERROR: Cannot open image file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }

    printf("OK (%dx%d).\n", width, height);

    // Agora criamos objetos na GPU com OpenGL para armazenar a textura
    GLuint texture_id;
    GLuint sampler_id;
    glGenTextures(1, &texture_id);
    glGenSamplers(1, &sampler_id);

    // Veja slides 95-96 do documento Aula_20_Mapeamento_de_Texturas.pdf
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Parâmetros de amostragem da textura.
    glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Agora enviamos a imagem lida do disco para a GPU
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    GLuint textureunit = g_NumLoadedTextures;
    glActiveTexture(GL_TEXTURE0 + textureunit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindSampler(textureunit, sampler_id);

    stbi_image_free(data);

    g_NumLoadedTextures += 1;
}



// Geração das instâncias ======================================================================================================
void GenerateObjectInstances(glm::vec4 camera_lookat_l)
{
    // Inicialização de um objeto
    glm::mat4 model = Matrix_Identity(); // Transformação identidade de modelagem

    // For the first sphere:
    model = Matrix_Translate(camera_lookat_l.x,camera_lookat_l.y,camera_lookat_l.z)
            * Matrix_Scale(0.05f,0.05f,0.05f);
    ObjectInstance("Central Sphere", model, CENTRAL_SPHERE);

    // //Desenhamos o modelo da esfera
    model = Matrix_Translate(-2.0f,0.0f,0.5f)
            * Matrix_Scale(1.0f,1.0f,1.0f);
    ObjectInstance("World", model, SPHERE);

    // Desenhamos outra instancia da esfera
    model = Matrix_Translate(-2.0f,-2.0f,0.0f)
            * Matrix_Scale(0.4f,0.4f,0.4f);
    ObjectInstance("World2", model, SPHERE);

    // Desenhamos o modelo do coelho
    model = Matrix_Translate(1.0f,0.0f,0.0f)
        * Matrix_Scale(0.3f,0.3f,0.3f);
    ObjectInstance("Bunny1", model, BUNNY);

    // Desenhamos outra instancia do coelho
    model = Matrix_Translate(0.8f,-0.5f,0.5f)
            * Matrix_Scale(0.2f,0.2f,0.2f)
            * Matrix_Rotate_X(0.1f);
    ObjectInstance("Bunny2", model, BUNNY);

    // Desenhamos o plano do chão
    model = Matrix_Translate(0.0f,-1.1f,0.0f);
    ObjectInstance("Plane", model, PLANE);

    // 90 degrees in radians
    float ninety_degrees = 1.5708f;
    // Desenhamos o modelo da vaca
    model = Matrix_Translate(-0.4f,-0.5f,-0.6f)
            * Matrix_Rotate_Y(ninety_degrees)
            * Matrix_Rotate_X(1.0f)
            * Matrix_Rotate_Z(0.7f);
    ObjectInstance("Cow", model, COW);

    // Desenhamos o modelo do cubo
    model = Matrix_Translate(2.0f,0.0f,-0.7f);
    ObjectInstance("Cube", model, CUBE);

    // // Desenhamos o modelo do retangulo
    // model = Matrix_Translate(-3.0f,0.0f,0.7f);
    // ObjectInstance("Rectangle", model, RECTANGLE);

    // Desenhamos os eixos XYZ
    glm::mat4 model_origin = Matrix_Translate(0.0f,0.0f,0.0f);
    ObjectInstance("X Axis", model_origin, X_AXIS);
    ObjectInstance("Y Axis", model_origin, Y_AXIS);
    ObjectInstance("Z Axis", model_origin, Z_AXIS);
}

void SetupXYZAxesVAOAndVBO(GLuint &VAO_X_axis, GLuint &VAO_Y_axis, GLuint &VAO_Z_axis, GLuint &VBO_X_axis, GLuint &VBO_Y_axis, GLuint &VBO_Z_axis)
{
    GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
    GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"

    glm::vec4 x_axisVertices[] = {
    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  // Origin for X-axis
    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  // X-axis end
    };

    // Set up the X axis
    glGenVertexArrays(1, &VAO_X_axis);
    glBindVertexArray(VAO_X_axis);

    glGenBuffers(1, &VBO_X_axis);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_X_axis);
    glBufferData(GL_ARRAY_BUFFER, sizeof(x_axisVertices), x_axisVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0); // unbind


    glm::vec4 y_axisVertices[] = {
    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  // Origin for Y-axis
    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  // Y-axis end
    };

    // Set up the Y axis
    glGenVertexArrays(1, &VAO_Y_axis);
    glBindVertexArray(VAO_Y_axis);

    glGenBuffers(1, &VBO_Y_axis);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Y_axis);
    glBufferData(GL_ARRAY_BUFFER, sizeof(y_axisVertices), y_axisVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0); // unbind

    glm::vec4 z_axisVertices[] = {
    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),  // Origin for Z-axis
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),  // Z-axis end
    };

    // Set up the Z axis
    glGenVertexArrays(1, &VAO_Z_axis);
    glBindVertexArray(VAO_Z_axis);

    glGenBuffers(1, &VBO_Z_axis);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Z_axis);
    glBufferData(GL_ARRAY_BUFFER, sizeof(z_axisVertices), z_axisVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0); // unbind
}

void SetupRayVAOAndVBO()
{
    GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
    GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"

    glGenVertexArrays(1, &VAO_ray_id);
    glBindVertexArray(VAO_ray_id);
    
    glGenBuffers(1, &VBO_ray_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ray_id);

    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0); // unbind the VAO
}



// Funções de desenho ======================================================================================================
// Função que desenha um objeto armazenado em g_VirtualScene. Veja definição
// dos objetos na função BuildTrianglesAndAddToVirtualScene().
void DrawVirtualObject(const char* object_name)
{
    // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
    // vértices apontados pelo VAO criado pela função BuildTrianglesAndAddToVirtualScene(). Veja
    // comentários detalhados dentro da definição de BuildTrianglesAndAddToVirtualScene().
    glBindVertexArray(g_VirtualScene[object_name].vertex_array_object_id);

    // Setamos as variáveis "bbox_min" e "bbox_max" do fragment shader
    // com os parâmetros da axis-aligned bounding box (AABB) do modelo.
    glm::vec3 bbox_min = g_VirtualScene[object_name].bbox_min;
    glm::vec3 bbox_max = g_VirtualScene[object_name].bbox_max;
    glUniform4f(g_bbox_min_uniform, bbox_min.x, bbox_min.y, bbox_min.z, 1.0f);
    glUniform4f(g_bbox_max_uniform, bbox_max.x, bbox_max.y, bbox_max.z, 1.0f);

    // Pedimos para a GPU rasterizar os vértices dos eixos XYZ
    // apontados pelo VAO como linhas. Veja a definição de
    // g_VirtualScene[""] dentro da função BuildTrianglesAndAddToVirtualScene(), e veja
    // a documentação da função glDrawElements() em
    // http://docs.gl/gl3/glDrawElements.
    glDrawElements(
        g_VirtualScene[object_name].rendering_mode,
        g_VirtualScene[object_name].num_indices,
        GL_UNSIGNED_INT,
        (void*)(g_VirtualScene[object_name].first_index * sizeof(GLuint))
    );

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);

    if (g_drawBoundingBox)
    {
        glUniform1i(g_is_bounding_box_vertex_uniform, GL_TRUE);    // Habilita o rendering no shader_vertex.glsl
        glUniform1i(g_is_bounding_box_fragment_uniform, GL_TRUE);   // Habilita o rendering no shader_vertex.glsl
        glBindVertexArray(g_VirtualScene[object_name].bbox_vertex_array_object_id);
        glDrawArrays(GL_LINES, 0, 24);  // 24 for 12 lines * 2 vertices per line
        glBindVertexArray(0);
    }
    else
    {
        glUniform1i(g_is_bounding_box_vertex_uniform, GL_FALSE);    // Habilita o rendering no shader_vertex.glsl
        glUniform1i(g_is_bounding_box_fragment_uniform, GL_FALSE);   // Habilita o rendering no shader_vertex.glsl
    }
}

void DrawRay(glm::vec4 rayStartPoint, glm::vec4 rayDirection)
{
    // Calculate ray end point
    glm::vec4 rayEndPoint = rayStartPoint + rayDirection * g_rayLength;
    glm::vec4 g_rayVertices[2] = {rayStartPoint, rayEndPoint};

    glBindBuffer(GL_ARRAY_BUFFER, VBO_ray_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_rayVertices), g_rayVertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the VBO

    glBindVertexArray(VAO_ray_id);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0); // unbind the VAO after drawing
}


// Funções de controle ======================================================================================================
void SetObjectInformationWindowData()
{
    if (g_selectedObject != -1)
    {
        ObjectInstance objectInstance = g_ObjectInstances[g_selectedObject];

        // Pega o nome do objeto selecionado
        g_selectedObjectName = objectInstance.object_name;

        // Pega o centro do objeto selecionado

        SceneObject sceneObject = g_idToSceneObject[objectInstance.sceneObject_id];
        glm::vec4 sceneObject_bb_min = sceneObject.bbox_min;
        glm::vec4 sceneObject_bb_max = sceneObject.bbox_max;
        glm::vec4 sceneObject_center = (sceneObject_bb_min + sceneObject_bb_max) / 2.0f;
        g_selectedObjectCenter = g_ObjectInstances[g_selectedObject].model_matrix * sceneObject_center;

        // PEga a escala do objeto selecionado
        g_selectedObjectScale.x = norm(glm::vec4(g_ObjectInstances[g_selectedObject].model_matrix[0]));
        g_selectedObjectScale.y = norm(glm::vec4(g_ObjectInstances[g_selectedObject].model_matrix[1]));
        g_selectedObjectScale.z = norm(glm::vec4(g_ObjectInstances[g_selectedObject].model_matrix[2]));

        // Pega a rotação do objeto selecionado
        glm::quat rotationQuat = glm::quat_cast(g_ObjectInstances[g_selectedObject].model_matrix);
        glm::vec3 eulerAngles = glm::eulerAngles(rotationQuat);
        g_selectedObjectRotation = glm::vec4(glm::degrees(eulerAngles), 0.0f);
    }
    else
    {
        g_selectedObjectName = "NO OBJECT SELECTED";
        g_selectedObjectCenter = glm::vec4(0.0f,0.0f,0.0f,1.0f);
        g_selectedObjectScale = glm::vec4(0.0f,0.0f,0.0f,0.0f);
        g_selectedObjectRotation = glm::vec4(0.0f,0.0f,0.0f,0.0f);
    }
}


// ??????????? ======================================================================================================
// Esta função recebe um vértice com coordenadas de modelo p_model e passa o
// mesmo por todos os sistemas de coordenadas armazenados nas matrizes model,
// view, e projection; e escreve na tela as matrizes e pontos resultantes
// dessas transformações.
void TextRendering_ShowModelViewProjection(
    GLFWwindow* window,
    glm::mat4 projection,
    glm::mat4 view,
    glm::mat4 model,
    glm::vec4 p_model
)
{
    if ( !g_ShowInfoText )
        return;

    glm::vec4 p_world = model*p_model;
    glm::vec4 p_camera = view*p_world;
    glm::vec4 p_clip = projection*p_camera;
    glm::vec4 p_ndc = p_clip / p_clip.w;

    float pad = TextRendering_LineHeight(window);

    TextRendering_PrintString(window, " Model matrix             Model     In World Coords.", -1.0f, 1.0f-pad, 1.0f);
    TextRendering_PrintMatrixVectorProduct(window, model, p_model, -1.0f, 1.0f-2*pad, 1.0f);

    TextRendering_PrintString(window, "                                        |  ", -1.0f, 1.0f-6*pad, 1.0f);
    TextRendering_PrintString(window, "                            .-----------'  ", -1.0f, 1.0f-7*pad, 1.0f);
    TextRendering_PrintString(window, "                            V              ", -1.0f, 1.0f-8*pad, 1.0f);

    TextRendering_PrintString(window, " View matrix              World     In Camera Coords.", -1.0f, 1.0f-9*pad, 1.0f);
    TextRendering_PrintMatrixVectorProduct(window, view, p_world, -1.0f, 1.0f-10*pad, 1.0f);

    TextRendering_PrintString(window, "                                        |  ", -1.0f, 1.0f-14*pad, 1.0f);
    TextRendering_PrintString(window, "                            .-----------'  ", -1.0f, 1.0f-15*pad, 1.0f);
    TextRendering_PrintString(window, "                            V              ", -1.0f, 1.0f-16*pad, 1.0f);

    TextRendering_PrintString(window, " Projection matrix        Camera                    In NDC", -1.0f, 1.0f-17*pad, 1.0f);
    TextRendering_PrintMatrixVectorProductDivW(window, projection, p_camera, -1.0f, 1.0f-18*pad, 1.0f);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glm::vec2 a = glm::vec2(-1, -1);
    glm::vec2 b = glm::vec2(+1, +1);
    glm::vec2 p = glm::vec2( 0,  0);
    glm::vec2 q = glm::vec2(width, height);

    glm::mat4 viewport_mapping = Matrix(
        (q.x - p.x)/(b.x-a.x), 0.0f, 0.0f, (b.x*p.x - a.x*q.x)/(b.x-a.x),
        0.0f, (q.y - p.y)/(b.y-a.y), 0.0f, (b.y*p.y - a.y*q.y)/(b.y-a.y),
        0.0f , 0.0f , 1.0f , 0.0f ,
        0.0f , 0.0f , 0.0f , 1.0f
    );

    TextRendering_PrintString(window, "                                                       |  ", -1.0f, 1.0f-22*pad, 1.0f);
    TextRendering_PrintString(window, "                            .--------------------------'  ", -1.0f, 1.0f-23*pad, 1.0f);
    TextRendering_PrintString(window, "                            V                           ", -1.0f, 1.0f-24*pad, 1.0f);

    TextRendering_PrintString(window, " Viewport matrix           NDC      In Pixel Coords.", -1.0f, 1.0f-25*pad, 1.0f);
    TextRendering_PrintMatrixVectorProductMoreDigits(window, viewport_mapping, p_ndc, -1.0f, 1.0f-26*pad, 1.0f);
}

// set makeprg=cd\ ..\ &&\ make\ run\ >/dev/null
// vim: set spell spelllang=pt_br :
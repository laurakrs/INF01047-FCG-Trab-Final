#include "GUI.h"

#include "globals.h"
#include "SceneInformation.h"
#include "ObjectInstance.h"


// Funções da GUI ======================================================================================================
void InitializeGUI(GLFWwindow* window)
{
    // Criação da GUI
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(g_startWindowWidth), static_cast<float>(g_startWindowHeight));
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void GenerateGUIWindows()
{
    // Inicia o frame da Dear ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    // Define a posição e tamanho da janela da GUI que adiciona instâncias de novos objetos
    float addNewInstanceWindowWidth = 220.0f;
    float addNewInstanceWindowHeight = 150.0f;
    ImVec2 addNewInstanceWindowSize = ImVec2(addNewInstanceWindowWidth, addNewInstanceWindowHeight);

    float offsetFromScreenLeftSide = 10.0f;
    float offsetFromScreenTop = 10.0f;
    ImVec2 addNewInstanceWindowPosition = ImVec2(offsetFromScreenLeftSide, offsetFromScreenTop);

    CreateAddNewInstanceWindow(addNewInstanceWindowSize, addNewInstanceWindowPosition);

    // Define a posição e tamanho da janela da GUI de debug
    float debugWindowWidth = 440.0f;
    float debugWindowHeight = 150.0f;
    ImVec2 debugWindowSize = ImVec2(debugWindowWidth, debugWindowHeight);
    ImVec2 debugWindowPosition = addNewInstanceWindowPosition;
    float spacingBetweenWindows = 10.0f;
    debugWindowPosition.x += addNewInstanceWindowSize.x + spacingBetweenWindows;

    CreateDebugWindow(debugWindowSize, debugWindowPosition);

    // Define a posição e tamanho da janela da GUI que escolhe entre proje;'ao perspectiva ou ortográfica
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    float projectionSettingsWindowWidth = 150.0f;
    float projectionSettingsWindowHeight = 58.0f;
    ImVec2 projectionSettingsWindowSize = ImVec2(projectionSettingsWindowWidth, projectionSettingsWindowHeight);

    float offsetFromScreenRightSide = -10.0f;
    float offsetFromScreenBottom = -10.0f;
    ImVec2 smallWindowPos = ImVec2(screenSize.x - projectionSettingsWindowSize.x + offsetFromScreenRightSide, screenSize.y - projectionSettingsWindowSize.y + offsetFromScreenBottom);
    
    CreateProjectionSettingsWindow(projectionSettingsWindowSize, smallWindowPos);

    // Renderiza a GUI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void CreateAddNewInstanceWindow(ImVec2 addNewInstanceWindowSize, ImVec2 addNewInstanceWindowPosition)
{
    ImGui::SetNextWindowSize(addNewInstanceWindowSize);
    ImGui::SetNextWindowPos(addNewInstanceWindowPosition);

    static bool show_window = true;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGui::Begin("Adicionar itens na cena", &show_window, window_flags);

    static bool isSphereSelected = false; // Store selection state for the first selectable
    if (ImGui::Selectable("Esfera", isSphereSelected))
    {
        // isSphereSelected = !isSphereSelected; // Toggle the selection state
    }

    static bool isCuboidSelected = false; // Store selection state for the first selectable
    if (ImGui::Selectable("Cubóide", isCuboidSelected))
    {
        // isCuboidSelected = !isCuboidSelected; // Toggle the selection state
    }
    
    static bool isCowSelected = false; // Store selection state for the first selectable
    if (ImGui::Selectable("Vaca", isCowSelected))
    {
        // isCowSelected = !isCowSelected; // Toggle the selection state
    }

    static bool isBunnySelected = false; // Store selection state for the first selectable
    if (ImGui::Selectable("Coelho", isBunnySelected))
    {
        // isBunnySelected = !isBunnySelected; // Toggle the selection state
    }

    static bool isRectangleSelected = false; // Store selection state for the first selectable
    if (ImGui::Selectable("Retângulo", isRectangleSelected))
    {
        // isRectangleSelected = !isRectangleSelected; // Toggle the selection state
    }

    if (ImGui::Selectable("Teste"))
    {
        // isRectangleSelected = !isRectangleSelected; // Toggle the selection state
    }


    ImGui::End();
}

void CreateDebugWindow(ImVec2 debugWindowSize, ImVec2 debugWindowPosition)
{
    ImGui::SetNextWindowSize(debugWindowSize);
    ImGui::SetNextWindowPos(debugWindowPosition);

    static bool show_window = true;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    ImGui::Begin("Debug", &show_window, window_flags);

    glm::vec3 cameraPosition = SceneInformation::camera_position_c;

    // show g_glfwLastRayCursorPosX
    ImGui::Text("==================================================");
    ImGui::Text("g_glfwLastRayCursorPosX: %.3f", g_glfwLastRayCursorPosX);
    ImGui::Text("g_NDCGlfwLastRayCursorPosX: %.3f", g_NDCGlfwLastRayCursorPosX);
    ImGui::Text("--------------------------------------------------");
    ImGui::Text("g_glfwLastRayCursorPosY: %.3f", g_glfwLastRayCursorPosY);
    ImGui::Text("g_NDCGlfwLastRayCursorPosY: %.3f", g_NDCGlfwLastRayCursorPosY);
    ImGui::Text("==================================================");

    // print g_rayClip, g_rayEye, g_rayWorld, g_rayDirection
    ImGui::Text("rayClip: x=%.3f, y=%.3f, z=%.3f w=%.3f", g_rayClip.x, g_rayClip.y, g_rayClip.z, g_rayClip.w);
    ImGui::Text("rayEye: x=%.3f, y=%.3f, z=%.3f w=%.3f", g_rayEye.x, g_rayEye.y, g_rayEye.z, g_rayEye.w);
    ImGui::Text("rayWorld: x=%.3f, y=%.3f, z=%.3f w=%.3f", g_rayWorld.x, g_rayWorld.y, g_rayWorld.z, g_rayWorld.w);
    ImGui::Text("rayDirection: x=%.3f, y=%.3f, z=%.3f w=%.3f", g_rayDirection.x, g_rayDirection.y, g_rayDirection.z, g_rayDirection.w);




    ImGui::Text("cameraPosition: x=%.3f, y=%.3f, z=%.3f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
    ImGui::Text("rayEndPoint: x=%.3f, y=%.3f, z=%.3f w=%.3f", g_rayEndPoint.x, g_rayEndPoint.y, g_rayEndPoint.z, g_rayEndPoint.w);
    ImGui::Text("Window Width: %.3f", g_actualWindowWidth);
    ImGui::Text("Window Height: %.3f", g_actualWindowHeight);

    // Print all the keys, object ids and object names on std::map<int, ObjectInstance> g_ObjectInstances;
    for (auto& object : g_ObjectInstances)
    {
        ImGui::Text("Object %d / id: %d - %s", object.first, object.second.object_id, object.second.object_name.c_str());

        // if object id is 9, 10 or 11, print the model matrix from the object
        if (object.second.object_id == 9 || object.second.object_id == 10 || object.second.object_id == 11)
        {
            ImGui::Text("%.3f %.3f %.3f %.3f", object.second.model_matrix[0][0], object.second.model_matrix[0][1], object.second.model_matrix[0][2], object.second.model_matrix[0][3]);
            ImGui::Text("%.3f %.3f %.3f %.3f", object.second.model_matrix[1][0], object.second.model_matrix[1][1], object.second.model_matrix[1][2], object.second.model_matrix[1][3]);
            ImGui::Text("%.3f %.3f %.3f %.3f", object.second.model_matrix[2][0], object.second.model_matrix[2][1], object.second.model_matrix[2][2], object.second.model_matrix[2][3]);
            ImGui::Text("%.3f %.3f %.3f %.3f", object.second.model_matrix[3][0], object.second.model_matrix[3][1], object.second.model_matrix[3][2], object.second.model_matrix[3][3]);
        }
    }

    ImGui::Text("==================================================");
    ImGui::Text("g_error: %s", g_error.c_str());

    ImGui::End();
}

void CreateProjectionSettingsWindow(ImVec2 projectionWindowSize, ImVec2 projectionWindowPosition)
{
    ImGui::SetNextWindowSize(projectionWindowSize);
    ImGui::SetNextWindowPos(projectionWindowPosition);

    // Criação da janela
    ImGuiWindowFlags projectionWindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGui::Begin("##NoTitle", NULL, projectionWindowFlags);

    // Inicializa o radio button
    static int radioValue = 0;

    // Radio button de Perspective
    if (ImGui::RadioButton("##radio1", &radioValue, 0)) 
    {
        g_UsePerspectiveProjection = true;
    }
    ImGui::SameLine();
    ImGui::Text("Perspective");

    // Radio button de Orthographic
    if (ImGui::RadioButton("##radio2", &radioValue, 1)) 
    {
        g_UsePerspectiveProjection = false;
    }
    ImGui::SameLine();
    ImGui::Text("Orthographic");

    ImGui::End();
}

void DisposeGUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


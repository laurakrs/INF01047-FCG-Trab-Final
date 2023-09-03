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

    // Define a posição e tamanho da janela que exibe informações do objeto selecionado
    float objectInformationWindowWidth = 440.0f;
    float objectInformationWindowHeight = 200.0f;
    ImVec2 objectInformationWindowSize = ImVec2(objectInformationWindowWidth, objectInformationWindowHeight);
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    float offsetFromScreenBottom = 10.0f;
    float objectInformationWindowXPosition = addNewInstanceWindowPosition.x;
    float objectInformationWindowYPosition = screenSize.y - objectInformationWindowSize.y - offsetFromScreenBottom;
    ImVec2 objectInformationWindowPosition = ImVec2(objectInformationWindowXPosition, objectInformationWindowYPosition);
    CreateObjectInformationWindow(objectInformationWindowSize, objectInformationWindowPosition);

    // Define a posição e tamanho da janela da GUI de debug
    float debugWindowWidth = 440.0f;
    float debugWindowHeight = 600.0f;
    ImVec2 debugWindowSize = ImVec2(debugWindowWidth, debugWindowHeight);
    ImVec2 debugWindowPosition = addNewInstanceWindowPosition;
    float spacingBetweenWindows = 10.0f;
    debugWindowPosition.y += addNewInstanceWindowSize.y + spacingBetweenWindows;
    CreateDebugWindow(debugWindowSize, debugWindowPosition);

    // Define a posição e tamanho da janela da GUI que escolhe entre projeção perspectiva ou ortográfica
    float projectionSettingsWindowWidth = 150.0f;
    float projectionSettingsWindowHeight = 58.0f;
    ImVec2 projectionSettingsWindowSize = ImVec2(projectionSettingsWindowWidth, projectionSettingsWindowHeight);
    float offsetFromScreenRightSide = -10.0f;
    float projectionSettingsWindowXPosition = screenSize.x - projectionSettingsWindowSize.x + offsetFromScreenRightSide;
    float projectionSettingsWindowYPosition = screenSize.y - projectionSettingsWindowSize.y - offsetFromScreenBottom;
    ImVec2 projectionSettingsWindowPos = ImVec2(projectionSettingsWindowXPosition, projectionSettingsWindowYPosition);
    CreateProjectionSettingsWindow(projectionSettingsWindowSize, projectionSettingsWindowPos);

    // Define a posição e tamanho da janela da GUI que escolhe entre mostrar ou não as bounding boxes
    float showBoundingBoxesWindowWidth = 150.0f;
    float showBoundingBoxesWindowHeight = 35.0f;
    ImVec2 showBoundingBoxesWindowSize = ImVec2(showBoundingBoxesWindowWidth, showBoundingBoxesWindowHeight);
    float offsetFromProjectionSettingsWindow = 10.0f;
    float showBoundingBoxesWindowXPosition = projectionSettingsWindowXPosition;
    float showBoundingBoxesWindowYPosition = projectionSettingsWindowYPosition - showBoundingBoxesWindowSize.y - offsetFromProjectionSettingsWindow;
    ImVec2 showBoundingBoxesWindowPos = ImVec2(showBoundingBoxesWindowXPosition, showBoundingBoxesWindowYPosition);
    CreateShowBoundingBoxesWindow(showBoundingBoxesWindowSize, showBoundingBoxesWindowPos);

    // Define a posição e tamanho da janela da GUI que escolhe entre mostrar ou não os raios quando há um clique do mouse
    float drawMouseRayWindowWidth = 150.0f;
    float drawMouseRayWindowHeight = 35.0f;
    ImVec2 drawMouseRayWindowSize = ImVec2(drawMouseRayWindowWidth, drawMouseRayWindowHeight);
    float offsetFromShowBoundingBoxesWindow = 10.0f;
    float drawMouseRayWindowXPosition = showBoundingBoxesWindowXPosition;
    float drawMouseRayWindowYPosition = showBoundingBoxesWindowYPosition - drawMouseRayWindowSize.y - offsetFromShowBoundingBoxesWindow;
    ImVec2 drawMouseRayWindowPos = ImVec2(drawMouseRayWindowXPosition, drawMouseRayWindowYPosition);
    CreateDrawMouseRayWindow(drawMouseRayWindowSize, drawMouseRayWindowPos);

    // Define a posição e tamanho da janela da GUI que escolhe entre mostrar a animação de pick
    float pickAnimationWindowWidth = 150.0f;
    float pickAnimationWindowHeight = 35.0f;
    ImVec2 pickAnimationWindowSize = ImVec2(pickAnimationWindowWidth, pickAnimationWindowHeight);
    float offsetFromDrawMouseRayWindow = 10.0f;
    float pickAnimationWindowXPosition = drawMouseRayWindowXPosition;
    float pickAnimationWindowYPosition = drawMouseRayWindowYPosition - pickAnimationWindowSize.y - offsetFromDrawMouseRayWindow;
    ImVec2 pickAnimationWindowPos = ImVec2(pickAnimationWindowXPosition, pickAnimationWindowYPosition);
    CreatePickAnimationWindow(pickAnimationWindowSize, pickAnimationWindowPos);

    // Renderiza a GUI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void CreateAddNewInstanceWindow(ImVec2 windowSize, ImVec2 windowPosition)
{
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(windowPosition);

    static bool show_window = true;
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGui::Begin("Add items to scene", &show_window, windowFlags);

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

void CreateObjectInformationWindow(ImVec2 windowSize, ImVec2 windowPosition)
{
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(windowPosition);

    static bool show_window = true;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    ImGui::Begin("Object Information", &show_window, window_flags);
    ImGui::Text("%s", g_selectedObjectName.c_str());
    ImGui::Text("==================================================");
    ImGui::Text("Center Coordinates: x=%.3f, y=%.3f, z=%.3f", g_selectedObjectCenter.x, g_selectedObjectCenter.y, g_selectedObjectCenter.z);
    ImGui::Text("Scale: x=%.3f, y=%.3f, z=%.3f", g_selectedObjectScale.x, g_selectedObjectScale.y, g_selectedObjectScale.z);
    ImGui::Text("Rotation: x=%.3f, y=%.3f, z=%.3f", g_selectedObjectRotation.x, g_selectedObjectRotation.y, g_selectedObjectRotation.z);
    
    ImGui::End();
}

void CreateDebugWindow(ImVec2 windowSize, ImVec2 windowPosition)
{
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(windowPosition);

    static bool show_window = true;
    // ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    // ImGui::Begin("Debug", &show_window, window_flags);
    ImGui::Begin("Debug", &show_window);

    // show g_rayOrigin
    ImGui::Text("g_rayOrigin: x=%.3f, y=%.3f, z=%.3f", g_rayOrigin.x, g_rayOrigin.y, g_rayOrigin.z);
    ImGui::Text("g_rayDirection: x=%.3f, y=%.3f, z=%.3f", g_rayDirection.x, g_rayDirection.y, g_rayDirection.z);
    
    // show SceneInformation::camera_view_vector
    ImGui::Text("camera_view_vector: x=%.3f, y=%.3f, z=%.3f", SceneInformation::camera_view_vector.x, SceneInformation::camera_view_vector.y, SceneInformation::camera_view_vector.z);

    // show g_isScaleEqualOnAllDirections
    ImGui::Text("g_isScaleEqualOnAllDirections: %s", g_isScaleEqualOnAllDirections ? "true" : "false");
    ImGui::Text("g_toSphere: x=%.3f, y=%.3f, z=%.3f", g_toSphere.x, g_toSphere.y, g_toSphere.z);
    ImGui::Text("g_t: %.3f", g_t);
    ImGui::Text("g_closestPoint: x=%.3f, y=%.3f, z=%.3f", g_closestPoint.x, g_closestPoint.y, g_closestPoint.z);
    ImGui::Text("g_centerToPoint: x=%.3f, y=%.3f, z=%.3f", g_centerToPoint.x, g_centerToPoint.y, g_centerToPoint.z);

    // show g_distSquared, g_radiusSquared, g_t1, g_t2;
    ImGui::Text("g_distSquared: %.3f", g_distSquared);
    ImGui::Text("g_radiusSquared: %.3f", g_radiusSquared);
    ImGui::Text("g_t1: %.3f", g_t1);
    ImGui::Text("g_t2: %.3f", g_t2);


    ImGui::End();
}

void CreateProjectionSettingsWindow(ImVec2 windowSize, ImVec2 windowPosition)
{
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(windowPosition);

    // Criação da janela
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGui::Begin("ProjectionSettingsWindow", NULL, windowFlags);

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

void CreateShowBoundingBoxesWindow(ImVec2 windowSize, ImVec2 windowPosition)
{
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(windowPosition);

    // Criação da janela
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGui::Begin("ShowBoundingBoxesWindow", NULL, windowFlags);

    ImGui::Checkbox("Show B. Boxes", &g_drawBoundingBox);

    ImGui::End();
}

void CreateDrawMouseRayWindow(ImVec2 windowSize, ImVec2 windowPosition)
{
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(windowPosition);

    // Criação da janela
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGui::Begin("DrawMouseRayWindow", NULL, windowFlags);

    ImGui::Checkbox("Draw mouse ray", &g_drawMouseRay);

    ImGui::End();
}

void CreatePickAnimationWindow(ImVec2 windowSize, ImVec2 windowPosition)
{
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(windowPosition);

    // Criação da janela
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGui::Begin("PickAnimationWindow", NULL, windowFlags);

    ImGui::Checkbox("Pick animation", &g_pickAnimation);

    ImGui::End();
}

void DisposeGUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


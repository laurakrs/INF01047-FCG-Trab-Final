#pragma once

// Interface gráfica
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Funções da GUI
void InitializeGUI(GLFWwindow* window);
void GenerateGUIWindows();
void CreateAddNewInstanceWindow(ImVec2 addNewInstanceWindowSize, ImVec2 addNewInstanceWindowPosition);
void CreateDebugWindow(ImVec2 debugWindowSize, ImVec2 debugWindowPosition);
void CreateProjectionSettingsWindow(ImVec2 projectionWindowSize, ImVec2 projectionWindowPosition);
void CreateShowBoundingBoxesWindow(ImVec2 projectionWindowSize, ImVec2 projectionWindowPosition);
void CreateDrawMouseRayWindow(ImVec2 windowSize, ImVec2 windowPosition);
void DisposeGUI();
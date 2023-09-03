#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <GLFW/glfw3.h>
#include <limits>
#include <cstdio>
#include "globals.h"
#include "imgui/imgui_impl_glfw.h"
#include "ObjectInstance.h"
#include "SceneInformation.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ErrorCallback(int error, const char* description);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

#endif // CALLBACKS_H

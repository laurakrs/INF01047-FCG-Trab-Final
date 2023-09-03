#include "SceneObject.h"

// Definition of the global variable
std::map<std::string, SceneObject> g_VirtualScene;
std::map<int, SceneObject> g_idToSceneObject;
std::map<int, std::string> g_idToSceneObjectName;
std::map<std::string, int> g_nameToSceneObjectId;

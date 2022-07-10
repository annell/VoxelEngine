//
// Created by Stefan Annell on 2021-05-13.
//
#pragma once

// External
#include "imgui.h"

#include "ImGuizmo.h"
#include "ogt_vox.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// STL
#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <vector>

// Internal
#include "RenderingInterface.h"

#include "Engine.h"
#include "Factory.h"

#include "Delegate.h"
#include "EntityComponentSystem.h"
#include "LoadTexture.h"

#include "BaseComponents.h"
#include "CubeEntity.h"
#include "Shader.h"
#include "Text.h"
#include "VertexBufferArray.h"

#include "Factory.h"

#include "Camera.h"
#include "InputDefinitions.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "PlayerController.h"
#include "RenderingHandler.h"
#include "Scene.h"
#include "Window.h"

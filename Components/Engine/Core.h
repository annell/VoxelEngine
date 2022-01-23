//
// Created by Stefan Annell on 2021-05-13.
//
#pragma once

// External
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "ogt_vox.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "imgui.h"
#include "ImGuizmo.h"

// STL
#include <functional>
#include <string>
#include <vector>
#include <tuple>
#include <map>

// Internal
#include "Engine.h"
#include "Factory.h"

#include "Delegate.h"
#include "EntityComponentSystem.h"
#include "LoadTexture.h"

#include "Shader.h"
#include "VertexBufferArray.h"
#include "BaseComponents.h"
#include "CubeEntity.h"
#include "Text.h"

#include "Factory.h"

#include "Scene.h"
#include "Window.h"
#include "Camera.h"
#include "InputDefinitions.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "RenderingHandler.h"
#include "Logging.h"


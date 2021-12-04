#pragma once

// For use by Kross Clients

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "Kross/Core/Core.h"
#include "Kross/Core/Application.h"
#include "Kross/Core/Layer.h"
#include "Kross/Core/CoreLog.h"
#include "Kross/imgui/ImGuiLayer.h"
#include "Kross/Core/Input.h"	
#include "Kross/Core/Timestep.h"
#include "Kross/Core/Stack.h"

#include "Kross/Events/KeyCodes.h"
#include "Kross/Events/MouseButtonCodes.h"

#include "imgui/imgui.h"

#include "Kross/Scene/Scene.h"
#include "Kross/Scene/Components.h"
#include "Kross/Scene/Entity.h"

#include "Kross/Renderer/Renderer.h"
#include "Kross/Renderer/RenderTypes.h"
#include "Kross/Renderer/Renderer2D.h"

#include "Kross/Renderer/Cameras/Camera.h"
#include "Kross/Renderer/Cameras/Cameras/Orthographic.h"
#include "Kross/Renderer/Cameras/Cameras/Perspective.h"
#include "Kross/Renderer/Cameras/Controllers/OrthographicCtrl.h"
#include "Kross/Renderer/Cameras/Controllers/FirstPerson.h"
#include "Kross/Renderer/FrameBuffer.h"
#include "Kross/Renderer/Buffer.h"
#include "Kross/Renderer/Shaders.h"
#include "Kross/Renderer/Textures/Textures.h"
#include "Kross/Renderer/Textures/Atlas.h"
#include "Kross/Renderer/Textures/Array.h"
#include "Kross/Renderer/VertexArray.h"

#include "Kross/Core/EntryPoint.h"

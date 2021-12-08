#pragma once
#include "Kross/Core/Core.h"
#include "RendererAPI.h"
//#include "Cameras/Camera.h"
//#include "Shaders.h"

#include "RendererCommands.h"

namespace Kross {
	class KAPI Renderer {
	public:
		static constexpr uint8_t TWO_D = 2;
		static constexpr uint8_t THREE_D = 3;
		static constexpr uint8_t VOXEL = 4;
		static void Shutdown();
		static void Init(uint8_t type);
		//static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& va, const glm::mat4 transform = glm::mat4(1.0f));
		//static void Submit(const Ref<Shader>& shader, const Scope<VertexArray>& va, const glm::mat4 transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		static uint8_t s_uDims;
	};
}

#pragma once
#include "Kross/Core/Core.h"
#include "RendererAPI.h"
#include "Cameras/Camera.h"
#include "Shaders.h"

#include "RendererCommands.h"

namespace Kross {
	class KAPI Renderer {
	public:
		enum class Dimentions { D2 = 0, D3 };

	public:
		static void Shutdown();
		static void Init(Dimentions type);
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& va, const glm::mat4 transform = glm::mat4(1.0f));
		static void Submit(const Ref<Shader>& shader, const Scope<VertexArray>& va, const glm::mat4 transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		static Dimentions s_dDims;
	};
}

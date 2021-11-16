#pragma once
#include "Kross/Core/Core.h"
#include "VertexArray.h"

//#include "glm/fwd.hpp"
#include "glm/glm/fwd.hpp"

namespace Kross {

	class KAPI RendererAPI
	{
	public:
		enum class API {
			None = 0, OpenGL = 1, DirectX = 2
		};
		enum class Mode {
			Wireframe = 0, Fill = 1, Points = 2
		};

	public:
		virtual void Shutdown() const = 0;
		virtual void Init() const = 0;
		virtual void Clear() const = 0;
		virtual void SetClear(const glm::vec4& color) const = 0;
		virtual void SetClear(const unsigned int color) const = 0;
		virtual void DrawArrays(const Ref<VertexArray>& va) const = 0;
		virtual void DrawArrays(const Scope<VertexArray>& va, bool drawPoints) const = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& va) const = 0;
		virtual void DrawIndexed(const Scope<VertexArray>& va, bool drawPoints = false) const = 0;
		virtual void SetViewport(uint32_t width, uint32_t height) const = 0;
		virtual void SetMode(Mode mode) const = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}
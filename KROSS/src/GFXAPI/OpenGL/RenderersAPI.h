#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/RendererAPI.h"
#include "glm/glm.hpp"

namespace Kross::OpenGL {
	class KAPI RendererAPI final : public Kross::RendererAPI
	{
	public:
		virtual void Shutdown() const override;
		virtual void Init() const override;
		virtual void Clear() const override;
		virtual void SetClear(const glm::vec4& color) const override;
		virtual void SetClear(const unsigned int color) const override;
		virtual void DrawArrays(const Ref<VertexArray>& va) const override;
		virtual void DrawArrays(const Scope<VertexArray>& va, bool drawPoints) const override;
		virtual void DrawIndexed(const Ref<VertexArray>& va) const override;
		virtual void DrawIndexed(const Scope<VertexArray>& va, bool drawPoints) const override;
		virtual void SetViewport(uint32_t width, uint32_t height) const override;
		virtual void SetMode(Kross::RendererAPI::Mode mode) const override;
	};
}

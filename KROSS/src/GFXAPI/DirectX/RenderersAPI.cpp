#include <Kross_pch.h>
#include "RenderersAPI.h"

namespace Kross::DirectX {
	void RendererAPI::Shutdown() const
	{

	}
	void RendererAPI::Init() const
	{
	}
	void RendererAPI::Clear() const
	{
	}
	void RendererAPI::SetClear(const glm::vec4& color) const
	{
	}
	void RendererAPI::SetViewport(uint32_t width, uint32_t height) const
	{
	}
	void RendererAPI::DrawIndexed(const Ref<VertexArray>& va) const
	{
	}
	void RendererAPI::DrawIndexed(const Scope<VertexArray>& va) const
	{
	}
}

#include <Kross_pch.h>
#include "RenderersAPI.h"
#include "GLErrors.h"

namespace Kross::OpenGL {
	void RendererAPI::Shutdown() const
	{

	}
	void RendererAPI::Init() const
	{
		glCall(glEnable(GL_BLEND));
		glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		glCall(glEnable(GL_DEPTH_TEST));
	}
	void RendererAPI::Clear() const
	{
		glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
	void RendererAPI::SetClear(const glm::vec4& color) const
	{
		glCall(glClearColor(color.r, color.g, color.b, color.a));
	}
	void RendererAPI::SetClear(const unsigned int color) const
	{
		glCall(glClearColor(
			(((color & 0xff000000) >> 24) / 256.0f),
			(((color & 0x00ff0000) >> 16) / 256.0f),
			(((color & 0x0000ff00) >> 8) / 256.0f),
			(((color & 0x000000ff)) / 256.0f)
		));
	}
	void RendererAPI::DrawArrays(const Ref<VertexArray>& va) const
	{
	}
	void RendererAPI::DrawArrays(const Scope<VertexArray>& va, bool drawPoints) const
	{
		glDrawArrays(GL_POINTS, 0, va->GetVertex().size());
	}
	void RendererAPI::DrawIndexed(const Ref<VertexArray>& va) const
	{
		glCall(glDrawElements(GL_POINTS, va->GetIndex()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}
	void RendererAPI::DrawIndexed(const Scope<VertexArray>& va, bool drawPoints) const
	{
		glCall(glDrawElements(drawPoints ? GL_POINTS : GL_TRIANGLES, va->GetIndex()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}
	void RendererAPI::SetViewport(uint32_t width, uint32_t height) const
	{
		glCall(glViewport(0, 0, width, height));
	}
	void RendererAPI::SetMode(Kross::RendererAPI::Mode mode) const
	{
		switch (mode)
		{
		case Kross::RendererAPI::Mode::Wireframe: glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)); break;
		case Kross::RendererAPI::Mode::Fill: glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)); break;
		case Kross::RendererAPI::Mode::Points: glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)); break;
		}
	}
}

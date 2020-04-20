#include <Kross_pch.h>
#include "RenderersAPI.h"
#include "GLErrors.h"

namespace Kross::OpenGL {
	void RendererAPI::Shutdown() const
	{
		KROSS_CORE_INFO("[ {0} ] |||| Renderer Shutting Down", __FUNCTION__);
	}
	void RendererAPI::Init() const
	{
		GLerror();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}
	void RendererAPI::Clear() const
	{
		GLerror();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void RendererAPI::SetClear(const glm::vec4& color) const
	{
		GLerror();
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void RendererAPI::DrawIndexed(const Ref<VertexArray>& va, uint32_t indexCount) const
	{
		GLerror();
		va->Bind();
		glDrawArrays(GL_TRIANGLES, 0, va->GetIndex()->GetCount());
		//glDrawElements(GL_TRIANGLES, indexCount ? indexCount : va->GetIndex()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	void RendererAPI::DrawIndexed(const Scope<VertexArray>& va, uint32_t indexCount) const
	{
		GLerror();
		glDrawElements(GL_TRIANGLES, indexCount != 0 ? indexCount : va->GetIndex()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	void RendererAPI::SetViewport(uint32_t width, uint32_t height) const
	{
		GLerror();
		glViewport(0, 0, width, height);
	}
}

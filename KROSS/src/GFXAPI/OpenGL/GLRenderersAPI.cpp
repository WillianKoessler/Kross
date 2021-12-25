#include <Kross_pch.h>
#include "GLRenderersAPI.h"
#include "GLErrors.h"

namespace Kross::OpenGL {
	void RendererAPI::Shutdown() const {
		
	}
	void RendererAPI::Init() const {
		glCall(glEnable(GL_BLEND));
		glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		glCall(glEnable(GL_DEPTH_TEST));
	}
	void RendererAPI::Clear() const {
		glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
	void RendererAPI::SetClear(const glm::vec4& color) const {
		glCall(glClearColor(color.r, color.g, color.b, color.a));
	}
	void RendererAPI::SetClear(const unsigned int color) const {
		glCall(glClearColor(
			(((color & 0xff000000) >> 24) / 255.0f),
			(((color & 0x00ff0000) >> 16) / 255.0f),
			(((color & 0x0000ff00) >> 8) / 255.0f),
			(((color & 0x000000ff)) / 255.0f)
		));
	}
	void RendererAPI::DrawArrays(const Ref<VertexArray>& va) const {
	}
	void RendererAPI::DrawArrays(const Scope<VertexArray>& va, uint32_t indexCount, bool drawPoints) const {
		uint32_t size = indexCount ? indexCount : va->GetIndex()->GetCount();
		glCall(glDrawArrays(GL_POINTS, 0, va->GetIndex()->GetCount()));
	}
	void RendererAPI::DrawIndexed(const Ref<VertexArray>& va) const {
		glCall(glDrawElements(GL_POINTS, va->GetIndex()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}
	void RendererAPI::DrawIndexed(const Scope<VertexArray>& va, uint32_t indexCount, bool drawPoints) const {
		uint32_t size = indexCount ? indexCount : va->GetIndex()->GetCount();
		glCall(glDrawElements(drawPoints ? GL_POINTS : GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr));
		glCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	void RendererAPI::SetViewport(uint32_t width, uint32_t height) const {
		glCall(glViewport(0, 0, width, height));
	}
	void RendererAPI::SetMode(Kross::RendererAPI::Mode mode) const {
		switch (mode) {
			case Kross::RendererAPI::Mode::Wireframe: glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)); break;
			case Kross::RendererAPI::Mode::Fill: glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)); break;
			case Kross::RendererAPI::Mode::Points: glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)); break;
		}
	}
	void RendererAPI::BackCull(bool b) const {
		if (b) {
			glCall(glEnable(GL_CULL_FACE));
		} else {
			glCall(glDisable(GL_CULL_FACE));
		}
	}
}

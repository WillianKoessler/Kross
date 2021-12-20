#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/RendererAPI.h"
#include "Kross/Renderer/VertexArray.h"

namespace Kross {
	class KAPI RenderCommand
	{
	public:
		inline static void Shutdown()
		{
			s_RendererAPI->Shutdown();
			delete s_RendererAPI;
		}
		inline static void Init()
		{
			s_RendererAPI->Init();
		}
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}
		inline static void SetClear(const glm::vec4& color)
		{
			s_RendererAPI->SetClear(color);
		}
		inline static void SetClear(const unsigned int color)
		{
			s_RendererAPI->SetClear(color);
		}
		inline static void DrawArrays(const Ref<VertexArray>& va)
		{
			s_RendererAPI->DrawArrays(va);
		}
		inline static void DrawArrays(const Scope<VertexArray>& va, bool drawPoints = false)
		{
			s_RendererAPI->DrawArrays(va, drawPoints);
		}
		inline static void DrawIndexed(const Ref<VertexArray>& va)
		{
			s_RendererAPI->DrawIndexed(va);
		}
		inline static void DrawIndexed(const Scope<VertexArray>& va, uint32_t indexCount, bool drawPoints = false)
		{
			s_RendererAPI->DrawIndexed(va, indexCount, drawPoints);
		}
		inline static void OnWindowResize(uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(width, height);
		}
		inline static void SetMode(Kross::RendererAPI::Mode mode)
		{
			s_RendererAPI->SetMode(mode);
		}
		inline static void BackCull(bool v)
		{
			s_RendererAPI->BackCull(v);

		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}

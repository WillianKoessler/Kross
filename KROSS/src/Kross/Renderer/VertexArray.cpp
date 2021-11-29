#include "Kross_pch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "GFXAPI/OpenGL/VertexArrays.h"

namespace Kross {

	Ref<VertexArray> VertexArray::CreateRef()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		KROSS_CORE_WARN("Renderer API (None) is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return makeRef<OpenGL::VertexArray>();
		}
		KROSS_CORE_WARN("Renderer API (None) is not supported");
		return nullptr;
	}

	Scope<VertexArray> VertexArray::CreateScope()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		KROSS_CORE_WARN("Renderer API (None) is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return makeScope<OpenGL::VertexArray>();
		}
		KROSS_CORE_WARN("Renderer API (None) is not supported");
		return nullptr;
	}
}
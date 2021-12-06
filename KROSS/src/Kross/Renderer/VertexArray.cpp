#include "Kross_pch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "GFXAPI/OpenGL/GLVertexArrays.h"

namespace Kross {

	Ref<VertexArray> VertexArray::CreateRef(const char* name)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		KROSS_CORE_WARN("Renderer API (None) is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return makeRef<OpenGL::VertexArray>(name);
		}
		KROSS_CORE_WARN("Renderer API (None) is not supported");
		return nullptr;
	}

	Scope<VertexArray> VertexArray::CreateScope(const char* name)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		KROSS_CORE_WARN("Renderer API (None) is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return makeScope<OpenGL::VertexArray>(name);
		}
		KROSS_CORE_WARN("Renderer API (None) is not supported");
		return nullptr;
	}
}
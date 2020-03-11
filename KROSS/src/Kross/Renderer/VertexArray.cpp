#include "Kross_pch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "GFXAPI/OpenGL/VertexArrays.h"

namespace Kross {

	Ref<VertexArray> VertexArray::CreateRef()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		KROSS_MSGBOX("[Kross::VertexArray] Renderer API (None) is not supported", "Vertex Array", _ERROR_); return nullptr;
		case RendererAPI::API::OpenGL:		return makeRef<OpenGL::VertexArray>();
		}

		KROSS_MSGBOX("[Kross::VertexArray] Renderer API (None) is not supported", "Vertex Array", _ERROR_);
		return nullptr;
	}

	Scope<VertexArray> VertexArray::CreateScope()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		Log::WindowBox("[Kross::VertexArray] Renderer API (None) is not supported", "Vertex Array", _ERROR_); return nullptr;
		case RendererAPI::API::OpenGL:		return makeScope<OpenGL::VertexArray>();
		}

		KROSS_MSGBOX("[Kross::VertexArray] Renderer API (None) is not supported", "Vertex Array", _ERROR_);
		return nullptr;
	}

}
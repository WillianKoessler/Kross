#include "Kross_pch.h"
#include "FrameBuffers.h"
#include "Renderer.h"

#include "GFXAPI/OpenGL/FrameBuffer.h"

namespace Kross {
	Ref<FrameBuffers> FrameBuffers::CreateRef()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		KROSS_MSGBOX("[Kross::FrameBuffers] Renderer API (None) is not supported", "Vertex Array", _ERROR_); return nullptr;
		case RendererAPI::API::OpenGL:		return makeRef<OpenGL::FrameBuffer>();
		}

		KROSS_MSGBOX("[Kross::FrameBuffers] Renderer API (None) is not supported", "Vertex Array", _ERROR_);
		return nullptr;
	}

	Scope<FrameBuffers> FrameBuffers::CreateScope()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		Log::WindowBox("[Kross::FrameBuffers] Renderer API (None) is not supported", "Vertex Array", _ERROR_); return nullptr;
		case RendererAPI::API::OpenGL:		return makeScope<OpenGL::FrameBuffer>();
		}

		KROSS_MSGBOX("[Kross::FrameBuffers] Renderer API (None) is not supported", "Vertex Array", _ERROR_);
		return nullptr;
	}
}
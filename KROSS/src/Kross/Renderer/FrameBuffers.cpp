#include <Kross_pch.h>
#include "FrameBuffers.h"

#include "Renderer.h"

#include "GFXAPI/OpenGL/FrameBuffer.h"

namespace Kross {

	Ref<FrameBuffers> Kross::FrameBuffers::CreateRef(const FrameBufferSpec& specs)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    KROSS_MSGBOX_ERROR("Renderer API (None) is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:  return makeRef<Kross::OpenGL::FrameBuffer>(specs);
		}
		KROSS_MSGBOX_ERROR("Renderer API (None) is not supported Renderer API (None) is not supported", "FrameBuffer", _ERROR_);
		return nullptr;
	}

	Scope<FrameBuffers> Kross::FrameBuffers::CreateScope(const FrameBufferSpec& specs)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    KROSS_MSGBOX_ERROR("Renderer API (None) is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:  return makeScope<Kross::OpenGL::FrameBuffer>(specs);
		}
		KROSS_MSGBOX_ERROR("Renderer API (None) is not supported Renderer API (None) is not supported", "FrameBuffer", _ERROR_);
		return nullptr;
	}
}

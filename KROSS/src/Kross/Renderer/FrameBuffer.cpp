#include <Kross_pch.h>
#include "FrameBuffer.h"

#include "Renderer.h"

#include "GFXAPI/OpenGL/GLFrameBuffer.h"

namespace Kross {

	Ref<FrameBuffer> Kross::FrameBuffer::CreateRef(const FrameBufferSpec& specs)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    KROSS_MSGBOX_ERROR("Renderer API (None) is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:  return makeRef<Kross::OpenGL::FrameBuffer>(specs);
		}
		KROSS_MSGBOX_ERROR("Renderer API (None) is not supported Renderer API (None) is not supported", "FrameBuffer", _ERROR_);
		return nullptr;
	}

	Scope<FrameBuffer> Kross::FrameBuffer::CreateScope(const FrameBufferSpec& specs)
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

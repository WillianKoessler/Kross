#include "Kross_pch.h"
#include "FrameBuffer.h"
#include "Kross/Renderer/Renderer.h"
#include "GFXAPI/OpenGL/Texture/_FrameBuffer.h"
//#include "GFXAPI/DirectX/Texture/_FrameBuffer.h"

namespace Kross::Texture {
	Ref<FrameBuffer> FrameBuffer::CreateRef(const std::string& name, void* data)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeRef<Kross::OpenGL::Texture::FrameBuffer>(name, data);
			//case Kross::RendererAPI::API::DirectX:		return makeRef<Kross::DirectX::Texture::FrameBuffer>(name, data);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
	Scope<FrameBuffer> FrameBuffer::CreateScope(const std::string& name, void* data)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeScope<Kross::OpenGL::Texture::FrameBuffer>(name, data);
			//case Kross::RendererAPI::API::DirectX:		return makeScope<Kross::DirectX::Texture::FrameBuffer>(name, data);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
}
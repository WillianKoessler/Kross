#include "Kross_pch.h"
#include "Atlas.h"
#include "Kross/Renderer/Renderer.h"
#include "GFXAPI/OpenGL/Texture/_Atlas.h"

namespace Kross::Texture {
	Ref<T2DAtlas> T2DAtlas::CreateRef(const std::string& path, const std::string& name)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeRef<Kross::OpenGL::Texture::T2DAtlas>(path, name);
			//case Kross::RendererAPI::API::DirectX:		return makeRef<Kross::DirectX::Texture::T2D>(path);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
	Ref<T2DAtlas> T2DAtlas::CreateRef(uint32_t width, uint32_t height, void* data)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeRef<Kross::OpenGL::Texture::T2DAtlas>(width, height, data);
			//case Kross::RendererAPI::API::DirectX:		return makeRef<Kross::DirectX::Texture::T2D>(width, height, data);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
	Ref<T2DAtlas> T2DAtlas::CreateRef(uint32_t width, uint32_t height, const std::string& name, void* data)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeRef<Kross::OpenGL::Texture::T2DAtlas>(width, height, name, data);
			//case Kross::RendererAPI::API::DirectX:		return makeRef<Kross::DirectX::Texture::T2D>(width, height, data);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
	Scope<T2DAtlas> T2DAtlas::CreateScope(const std::string& path, const std::string& name)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeScope<Kross::OpenGL::Texture::T2DAtlas>(path, name);
			//case Kross::RendererAPI::API::DirectX:		return makeScope<Kross::DirectX::Texture::T2D>(path);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
	Scope<T2DAtlas> T2DAtlas::CreateScope(uint32_t width, uint32_t height, void* data)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeScope<Kross::OpenGL::Texture::T2DAtlas>(width, height, data);
			//case Kross::RendererAPI::API::DirectX:		return makeScope<Kross::DirectX::Texture::T2D>(width, height, data);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
	Scope<T2DAtlas> T2DAtlas::CreateScope(uint32_t width, uint32_t height, const std::string& name, void* data)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeScope<Kross::OpenGL::Texture::T2DAtlas>(width, height, name, data);
			//case Kross::RendererAPI::API::DirectX:		return makeRef<Kross::DirectX::Texture::T2D>(width, height, data);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
}
//Start (Changing Texture Atlas and Array on different files)

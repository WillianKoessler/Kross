#include "Kross_pch.h"
#include "Textures.h"
#include "Kross/Renderer/Renderer.h"
#include "GFXAPI/OpenGL/Texture/_Texture.h"
//#include "GFXAPI/DirectX/Texture.h"

namespace Kross::Texture {
	unsigned int Base::texSlotIndex = 0;

	const unsigned int Base::QueryMaxSlots()
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return -1;
		case Kross::RendererAPI::API::OpenGL:		return Kross::OpenGL::Texture::Base::QueryMaxSlots();
			//case Kross::RendererAPI::API::DirectX:		return makeRef<Kross::DirectX::Texture::T2D>(path);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return -1;
	}
	const unsigned int Base::IncSlot()
	{
		if (texSlotIndex + 1 >= QueryMaxSlots())
		{
			KROSS_CORE_WARN("[ {0} ] |||| Textures Slots Overflow.", __FUNCTION__);
			texSlotIndex = 1;
		}
		return texSlotIndex++;
	}
	void Base::ResetIndex()
	{
		texSlotIndex = 1;
	}
	const unsigned int Base::GetSlot()
	{
		return texSlotIndex;
	}
	Ref<T2D> T2D::CreateRef(const std::string& path, const std::string& name)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeRef<Kross::OpenGL::Texture::T2D>(path, name);
			//case Kross::RendererAPI::API::DirectX:		return makeRef<Kross::DirectX::Texture::T2D>(path);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
	Ref<T2D> T2D::CreateRef(uint32_t width, uint32_t height, void* data)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeRef<Kross::OpenGL::Texture::T2D>(width, height, data);
			//case Kross::RendererAPI::API::DirectX:		return makeRef<Kross::DirectX::Texture::T2D>(width, height, data);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
	Ref<T2D> T2D::CreateRef(uint32_t width, uint32_t height, const std::string& name, bool alpha, void* data)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeRef<Kross::OpenGL::Texture::T2D>(width, height, name, alpha, data);
			//case Kross::RendererAPI::API::DirectX:		return makeRef<Kross::DirectX::Texture::T2D>(width, height, data);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
	Ref<T2D> T2D::CreateRef(uint32_t width, uint32_t height, bool alpha, void* data)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeRef<Kross::OpenGL::Texture::T2D>(width, height, alpha, data);
			//case Kross::RendererAPI::API::DirectX:		return makeRef<Kross::DirectX::Texture::T2D>(width, height, alpha, data);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
	Scope<T2D> T2D::CreateScope(const std::string& path, const std::string& name)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeScope<Kross::OpenGL::Texture::T2D>(path, name);
			//case Kross::RendererAPI::API::DirectX:		return makeScope<Kross::DirectX::Texture::T2D>(path);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
	Scope<T2D> T2D::CreateScope(uint32_t width, uint32_t height, void* data)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeScope<Kross::OpenGL::Texture::T2D>(width, height, data);
			//case Kross::RendererAPI::API::DirectX:		return makeScope<Kross::DirectX::Texture::T2D>(width, height, data);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
	Scope<T2D> T2D::CreateScope(uint32_t width, uint32_t height, const std::string& name, bool alpha, void* data)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeScope<Kross::OpenGL::Texture::T2D>(width, height, name, alpha, data);
			//case Kross::RendererAPI::API::DirectX:		return makeRef<Kross::DirectX::Texture::T2D>(width, height, data);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
	Scope<T2D> T2D::CreateScope(uint32_t width, uint32_t height, bool alpha, void* data)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeScope<Kross::OpenGL::Texture::T2D>(width, height, alpha, data);
			//case Kross::RendererAPI::API::DirectX:		return makeScope<Kross::DirectX::Texture::T2D>(width, height, alpha, data);
		}

		KROSS_MSGBOX("Unknown Renderer API", __FUNCTION__, _ERROR_);
		return nullptr;
	}
}

#include "Kross_pch.h"
#include "Array.h"
#include "Kross/Renderer/Renderer.h"
#include "GFXAPI/OpenGL/Texture/_Array.h"

namespace Kross::Texture {
	Ref<T2DArray> T2DArray::CreateRef(const size_t size)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_CORE_ERROR("Renderer API (None) is not supported"); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeRef<Kross::OpenGL::Texture::T2DArray>(size);
			//case Kross::RendererAPI::API::DirectX:		return makeRef<Kross::DirectX::Texture::T2D>(width, height, data);
		}

		KROSS_CORE_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}
	Scope<T2DArray> T2DArray::CreateScope(const size_t size)
	{
		switch (Kross::Renderer::GetAPI())
		{
		case Kross::RendererAPI::API::None:		KROSS_CORE_ERROR("Renderer API (None) is not supported"); return nullptr;
		case Kross::RendererAPI::API::OpenGL:		return makeScope<Kross::OpenGL::Texture::T2DArray>(size);
			//case Kross::RendererAPI::API::DirectX:		return makeRef<Kross::DirectX::Texture::T2D>(width, height, data);
		}

		KROSS_CORE_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}
}
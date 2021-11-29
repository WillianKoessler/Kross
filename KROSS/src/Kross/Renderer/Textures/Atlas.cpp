#include <Kross_pch.h>
#include "Atlas.h"
#include "Kross/Renderer/Renderer.h"
#include "GFXAPI/OpenGL/Texture/GLAtlas.h"

namespace Kross::Texture {
	template<typename...Args>
	Ref<Kross::Texture::T2DAtlas> CreateReferenceImpl(Args&&...args)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		KROSS_CORE_WARN("Renderer API (None) is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Atlas>(std::forward<Args>(args)...);
		}
		KROSS_CORE_WARN("Renderer API (None) is not supported");
		return nullptr;
	}
	template<typename...Args>
	Scope<Kross::Texture::T2DAtlas> CreateScopeImpl(Args&&...args)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		KROSS_CORE_WARN("Renderer API (None) is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return makeScope<OpenGL::Atlas>(std::forward<Args>(args)...);
		}
		KROSS_CORE_WARN("Renderer API (None) is not supported");
		return nullptr;
	}
	Ref<Kross::Texture::T2DAtlas> T2DAtlas::CreateRef(Tex&& atlas_Texture, glm::vec2&& min, glm::vec2&& max)
	{
		return CreateReferenceImpl(atlas_Texture, min, max);
	}
	Ref<Kross::Texture::T2DAtlas> T2DAtlas::CreateRef(Tex&& atlas_Texture, glm::vec2&& cellSize, glm::vec2&& index, glm::vec2&& spriteSize)
	{
		return CreateReferenceImpl(atlas_Texture, cellSize, index, spriteSize);
	}
	Ref<Kross::Texture::T2DAtlas> T2DAtlas::CreateRef(const Tex& atlas_Texture, const glm::vec2& min, const glm::vec2& max)
	{
		return CreateReferenceImpl(atlas_Texture, min, max);
	}
	Ref<Kross::Texture::T2DAtlas> T2DAtlas::CreateRef(const Tex& atlas_Texture, const glm::vec2& cellSize, const glm::vec2& index, const glm::vec2& spriteSize)
	{
		return CreateReferenceImpl(atlas_Texture, cellSize, index, spriteSize);
	}
	Scope<Kross::Texture::T2DAtlas> T2DAtlas::CreateScope(Tex&& atlas_Texture, glm::vec2&& min, glm::vec2&& max)
	{
		return CreateScopeImpl(atlas_Texture, min, max);
	}
	Scope<Kross::Texture::T2DAtlas> T2DAtlas::CreateScope(Tex&& atlas_Texture, glm::vec2&& cellSize, glm::vec2&& index, glm::vec2&& spriteSize)
	{
		return CreateScopeImpl(atlas_Texture, cellSize, index, spriteSize);
	}
	Scope<Kross::Texture::T2DAtlas> T2DAtlas::CreateScope(const Tex& atlas_Texture, const glm::vec2& min, const glm::vec2& max)
	{
		return CreateScopeImpl(atlas_Texture, min, max);
	}
	Scope<Kross::Texture::T2DAtlas> T2DAtlas::CreateScope(const Tex& atlas_Texture, const glm::vec2& cellSize, const glm::vec2& index, const glm::vec2& spriteSize)
	{
		return CreateScopeImpl(atlas_Texture, cellSize, index, spriteSize);
	}
}

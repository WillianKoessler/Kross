#pragma once
#include "Kross/Renderer/Textures/Textures.h"
#include <glm/glm.hpp>

namespace Kross::Texture {
	class KAPI T2DAtlas
	{
	public:
		using Tex = Ref<Kross::Texture::T2D>;

		virtual void UpdateTexture(const glm::vec2& index) = 0;;
		virtual const glm::vec2* GetTexCoords() const = 0;
		virtual const Tex& GetTexture() const = 0;
		virtual const std::string& GetName() const = 0;

		static Ref<Kross::Texture::T2DAtlas> CreateRef(Tex&& atlas_Texture, glm::vec2&& min, glm::vec2&& max);
		static Ref<Kross::Texture::T2DAtlas> CreateRef(Tex&& atlas_Texture, glm::vec2&& cellSize, glm::vec2&& index, glm::vec2&& spriteSize);
		static Ref<Kross::Texture::T2DAtlas> CreateRef(const Tex& atlas_Texture, const glm::vec2& min, const glm::vec2& max);
		static Ref<Kross::Texture::T2DAtlas> CreateRef(const Tex& atlas_Texture, const glm::vec2& cellSize, const glm::vec2& index, const glm::vec2& spriteSize);
		static Scope<Kross::Texture::T2DAtlas> CreateScope(Tex&& atlas_Texture, glm::vec2&& min, glm::vec2&& max);
		static Scope<Kross::Texture::T2DAtlas> CreateScope(Tex&& atlas_Texture, glm::vec2&& cellSize, glm::vec2&& index, glm::vec2&& spriteSize);
		static Scope<Kross::Texture::T2DAtlas> CreateScope(const Tex& atlas_Texture, const glm::vec2& min, const glm::vec2& max);
		static Scope<Kross::Texture::T2DAtlas> CreateScope(const Tex& atlas_Texture, const glm::vec2& cellSize, const glm::vec2& index, const glm::vec2& spriteSize);
	};
}
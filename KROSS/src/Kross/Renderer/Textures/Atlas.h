#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/Textures/Textures.h"
#include <glm/glm.hpp>

namespace Kross::Texture {
	class KAPI T2DAtlas
	{
		std::string m_name;
		glm::vec2 m_TexCoords[4];
		glm::vec2 m_cellSize, m_spriteSize;
		Ref<Kross::Texture::T2D> m_Atlas;

		void calculate(const glm::vec2& min, const glm::vec2& max);
	public:
		T2DAtlas(Ref<Texture::T2D>&& atlas_Texture, glm::vec2&& min, glm::vec2&& max);
		T2DAtlas(Ref<Texture::T2D>&& atlas_Texture, glm::vec2&& cellSize, glm::vec2&& index, glm::vec2&& spriteSize);
		T2DAtlas(const Ref<Texture::T2D>& atlas_Texture, const glm::vec2& min, const glm::vec2& max);
		T2DAtlas(const Ref<Texture::T2D>& atlas_Texture, const glm::vec2& cellSize, const glm::vec2& index, const glm::vec2& spriteSize);

		void UpdateTexture(const glm::vec2& index);
		const glm::vec2* GetTexCoords() const;
		const Ref<Kross::Texture::T2D>& GetTexture() const;
		const std::string& GetName() const;
	};
}
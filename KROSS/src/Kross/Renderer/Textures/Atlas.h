#pragma once
#include "Kross/Renderer/Textures/Textures.h"
#include <glm/glm.hpp>

namespace Kross::Texture {
	class KAPI T2DAtlas
	{
		using Tex = Ref<Kross::Texture::T2D>;
		std::string m_name;
		glm::vec2 m_TexCoords[4];
		glm::vec2 m_cellSize, m_spriteSize;
		Tex m_Atlas;
		inline void calculate(const glm::vec2& min, const glm::vec2& max);
	public:
		T2DAtlas(Tex&& atlas_Texture, glm::vec2&& min, glm::vec2&& max);
		T2DAtlas(Tex&& atlas_Texture, glm::vec2&& cellSize, glm::vec2&& index, glm::vec2&& spriteSize);
		T2DAtlas(const Tex& atlas_Texture, const glm::vec2& min, const glm::vec2& max);
		T2DAtlas(const Tex& atlas_Texture, const glm::vec2& cellSize, const glm::vec2& index, const glm::vec2& spriteSize);
		void UpdateTexture(const glm::vec2& index);
		
		inline const glm::vec2* GetTexCoords() const { return m_TexCoords; }
		inline const Tex& GetTexture() const { return m_Atlas; }
		inline const std::string& GetName() const { return m_name; }
	};
}
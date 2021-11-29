#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/Textures/Atlas.h"

namespace Kross::OpenGL {
	class Atlas : public Texture::T2DAtlas
	{
		std::string m_name;
		glm::vec2 m_TexCoords[4];
		glm::vec2 m_cellSize, m_spriteSize;
		Ref<Kross::Texture::T2D> m_Atlas;
		void calculate(const glm::vec2& min, const glm::vec2& max);
	public:
		Atlas(Ref<Kross::Texture::T2D>&& atlas_Texture, glm::vec2&& min, glm::vec2&& max);
		Atlas(Ref<Kross::Texture::T2D>&& atlas_Texture, glm::vec2&& cellSize, glm::vec2&& index, glm::vec2&& spriteSize);
		Atlas(const Ref<Kross::Texture::T2D>& atlas_Texture, const glm::vec2& min, const glm::vec2& max);
		Atlas(const Ref<Kross::Texture::T2D>& atlas_Texture, const glm::vec2& cellSize, const glm::vec2& index, const glm::vec2& spriteSize);

		virtual void UpdateTexture(const glm::vec2& index) override;
		virtual const glm::vec2* GetTexCoords() const override;
		virtual const Ref<Kross::Texture::T2D>& GetTexture() const override;
		virtual const std::string& GetName() const override;
	};
}
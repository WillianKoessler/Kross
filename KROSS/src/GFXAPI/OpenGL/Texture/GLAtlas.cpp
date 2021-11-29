#include <Kross_pch.h>
#include "GLAtlas.h"

namespace Kross::OpenGL {
	void Atlas::calculate(const glm::vec2& min, const glm::vec2& max)
	{
		m_TexCoords[0] = min;
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = max;
		m_TexCoords[3] = { min.x, max.y };
	}
	Atlas::Atlas(Ref<Kross::Texture::T2D>&& atlas_Texture, glm::vec2&& min, glm::vec2&& max)
		: m_Atlas(atlas_Texture)
	{
		calculate(min, max);
	}
	Atlas::Atlas(Ref<Kross::Texture::T2D>&& atlas_Texture, glm::vec2&& cellSize, glm::vec2&& index, glm::vec2&& spriteSize)
		: m_name(atlas_Texture->GetName() + "_ATLAS"), m_Atlas(atlas_Texture), m_cellSize(cellSize), m_spriteSize(spriteSize)
	{
		calculate((index * cellSize) / m_Atlas->GetSize(), ((index + spriteSize) * cellSize) / m_Atlas->GetSize());
	}
	Atlas::Atlas(const Ref<Kross::Texture::T2D>& atlas_Texture, const glm::vec2& min, const glm::vec2& max)
		: m_Atlas(atlas_Texture)
	{
		calculate(min, max);
	}
	Atlas::Atlas(const Ref<Kross::Texture::T2D>& atlas_Texture, const glm::vec2& cellSize, const glm::vec2& index, const glm::vec2& spriteSize)
		: m_name(atlas_Texture->GetName() + "_ATLAS"), m_Atlas(atlas_Texture), m_cellSize(cellSize), m_spriteSize(spriteSize)
	{
		calculate((index * cellSize) / m_Atlas->GetSize(), ((index + spriteSize) * cellSize) / m_Atlas->GetSize());
	}
	void Atlas::UpdateTexture(const glm::vec2& index)
	{
		calculate((index * m_cellSize) / m_Atlas->GetSize(), ((index + m_spriteSize) * m_cellSize) / m_Atlas->GetSize());
	}
	const glm::vec2* Atlas::GetTexCoords() const
	{
		return m_TexCoords;
	}
	const Ref<Kross::Texture::T2D>& Atlas::GetTexture() const
	{
		return m_Atlas;
	}
	const std::string& Atlas::GetName() const
	{
		return m_name;
	}
}
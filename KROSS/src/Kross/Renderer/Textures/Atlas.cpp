#include "Kross_pch.h"
#include "Atlas.h"

namespace Kross::Texture {
	void T2DAtlas::calculate(const glm::vec2& min, const glm::vec2& max)
	{
		m_TexCoords[0] = min;
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = max;
		m_TexCoords[3] = { min.x, max.y };
	}
	T2DAtlas::T2DAtlas(const Tex& atlas_Texture, const glm::vec2& min, const glm::vec2& max) :
		m_Atlas(atlas_Texture)
	{
		calculate(min, max);
	}
	T2DAtlas::T2DAtlas(const Tex& atlas_Texture, const glm::vec2& cellSize, const glm::vec2& index, const glm::vec2& spriteSize) :
		m_name(atlas_Texture->GetName() + "_ATLAS"),
		m_Atlas(atlas_Texture),
		m_cellSize(cellSize),
		m_spriteSize(spriteSize)
	{
		calculate((index * cellSize) / m_Atlas->GetSize(), ((index + spriteSize) * cellSize) / m_Atlas->GetSize());
	}
	void T2DAtlas::UpdateTexture(const glm::vec2& index)
	{
		calculate((index * m_cellSize) / m_Atlas->GetSize(), ((index + m_spriteSize) * m_cellSize) / m_Atlas->GetSize());
	}
}

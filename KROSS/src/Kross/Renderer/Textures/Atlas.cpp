#include <Kross_pch.h>
#include "Atlas.h"
#include "Kross/Renderer/Renderer.h"

namespace Kross::Texture {
	void T2DAtlas::calculate(const glm::vec2 &min, const glm::vec2 &max)
	{
		m_TexCoords[0] = min;
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = max;
		m_TexCoords[3] = { min.x, max.y };
	}
	T2DAtlas::T2DAtlas(const char *name, Ref<Kross::Texture::T2D> &&T2DAtlas_Texture, glm::vec2 &&min, glm::vec2 &&max)
		: m_Atlas(T2DAtlas_Texture)
	{
		SetName(name);
		calculate(min, max);
	}
	T2DAtlas::T2DAtlas(const char *name, Ref<Kross::Texture::T2D> &&T2DAtlas_Texture, glm::vec2 &&cellSize, glm::vec2 &&index, glm::vec2 &&spriteSize)
		: m_Atlas(T2DAtlas_Texture), m_cellSize(cellSize), m_spriteSize(spriteSize)
	{
		SetName(name);
		calculate((index * cellSize) / m_Atlas->GetSize(), ((index + spriteSize) * cellSize) / m_Atlas->GetSize());
	}
	T2DAtlas::T2DAtlas(const char *name, const Ref<Kross::Texture::T2D> &T2DAtlas_Texture, const glm::vec2 &min, const glm::vec2 &max)
		: m_Atlas(T2DAtlas_Texture)
	{
		SetName(name);
		calculate(min, max);
	}
	T2DAtlas::T2DAtlas(const char *name, const Ref<Kross::Texture::T2D> &T2DAtlas_Texture, const glm::vec2 &cellSize, const glm::vec2 &index, const glm::vec2 &spriteSize)
		: m_Atlas(T2DAtlas_Texture), m_cellSize(cellSize), m_spriteSize(spriteSize)
	{
		SetName(name);
		calculate((index * cellSize) / m_Atlas->GetSize(), ((index + spriteSize) * cellSize) / m_Atlas->GetSize());
	}
	void T2DAtlas::GetSub(uint32_t index)
	{
		uint32_t w = (uint32_t)(m_Atlas->GetSize().x / m_cellSize.x);
		uint32_t h = (uint32_t)(m_Atlas->GetSize().y / m_cellSize.y);
		uint32_t t = w * h;
		if (index > t) { KROSS_WARN("index provided exceeds limit"); return; }
		GetSub({ index % w, index / h });
	}
	void T2DAtlas::GetSub(const glm::vec2 &index)
	{
		calculate((index * m_cellSize) / m_Atlas->GetSize(), ((index + m_spriteSize) * m_cellSize) / m_Atlas->GetSize());
	}
	const glm::vec2 *T2DAtlas::GetTexCoords() const
	{
		return m_TexCoords;
	}
	const Ref<Kross::Texture::T2D> &T2DAtlas::GetTexture() const
	{
		return m_Atlas;
	}
}

#pragma once
#include "Kross/Renderer/Textures/Textures.h"
#include "glm/glm.hpp"

namespace Kross::Texture {
	class KAPI T2DAtlas : public Base
	{
		unsigned int cellWidth, cellHeight;
		std::string name;
		Ref<Kross::Texture::T2D> atlas;

	public:

		T2DAtlas();
		void Load(const std::string& path, const std::string& name = "Default");
		inline void SetCellSize(uint8_t width, uint8_t height) { cellWidth = width; cellHeight = height; }
		uint8_t GetCellIndex(uint8_t x, uint8_t y);
		glm::vec2 GetCellCoord(uint8_t index);

		virtual uint32_t GetWidth() const override { return atlas->GetWidth(); }
		virtual uint32_t GetHeight() const override { return atlas->GetHeight(); }
		virtual const std::string& GetName() const override { return name; }
		virtual const unsigned int GetID() const override { return atlas->GetID(); }
		virtual const int GetCurrentSlot() const override { return atlas->GetCurrentSlot(); }
		virtual void SetData(void* data, uint32_t size) override { atlas->SetData(data, size); }
		virtual void Bind(uint32_t slot = 0) const override { atlas->Bind(slot); }
	};
}
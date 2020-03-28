#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/Textures/Atlas.h"

namespace Kross::OpenGL::Texture {
	class KAPI T2DAtlas final : public Kross::Texture::T2DAtlas
	{
	public:
		T2DAtlas::T2DAtlas(uint32_t width, uint32_t height, void* data = nullptr);
		T2DAtlas::T2DAtlas(uint32_t width, uint32_t height, const std::string& name, void* data = nullptr);
		T2DAtlas::T2DAtlas(const std::string& path, const std::string& name = "");

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;
		virtual const std::string& GetName() const override;
		virtual const unsigned int GetID() const override;
		virtual const int GetCurrentSlot() const override;

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		uint32_t
			_width = 0,
			_height = 0,
			_ID = UINT32_MAX,
			_Slot = 0,
			_internal_format = 0,
			_data_format = 0;
		std::string _name = "", _path = "";
	};
}

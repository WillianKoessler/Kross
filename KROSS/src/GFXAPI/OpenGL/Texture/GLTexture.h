#pragma once

#include <glm/glm.hpp>
#include "GFXAPI/OpenGL/GLErrors.h"
#include "Kross/Core/Core.h"
#include "Kross/Renderer/Textures/Textures.h"

namespace Kross::OpenGL::Texture {
	class Base final : public Kross::Texture::Base
	{
	public:
		static const int32_t QueryMaxSlots();
	};

	class T2D final : public Kross::Texture::T2D
	{
	public:
		T2D() { SetName("Unnamed_T2D"); }
		T2D(const char* name, const char* path);
		T2D(const char* name, uint32_t width, uint32_t height, uint8_t* data = nullptr);

		virtual void SetFilter(Filtering_Type filter) override;
		virtual void SetData(uint8_t* data, uint32_t size) override;
		virtual void ResetData() const override;
		virtual const Ref<uint8_t>& GetData() const override;
		virtual const char *GetPath() const override { return m_strPath; }

		virtual ~T2D() override;

		virtual uint32_t GetWidth() const override { return m_unWidth; }
		virtual uint32_t GetHeight() const override { return m_unHeight; }
		virtual glm::vec2 GetSize() const override { return glm::vec2((float)m_unWidth, (float)m_unHeight); }
		virtual uint32_t GetID() const override { return m_RendererID; }
		virtual int32_t GetCurrentSlot() const override { return m_CurrentSlot; }

		virtual void Bind(uint32_t slot = 0) const override;
	private:
		Ref<uint8_t> raw_data = nullptr;
		const char* m_strPath = nullptr;
		uint32_t m_RendererID = UINT32_MAX, m_unWidth = UINT32_MAX, m_unHeight = UINT32_MAX;
		uint8_t m_CurrentSlot = UINT8_MAX;
		int32_t m_unInternalFormat = INT32_MAX;
		uint32_t m_unDataFormat = UINT32_MAX;
	};
}

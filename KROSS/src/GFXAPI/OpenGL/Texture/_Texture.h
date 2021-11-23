#pragma once

#include <glm/glm.hpp>
#include "GFXAPI/OpenGL/GLErrors.h"
#include "Kross/Core/Core.h"
#include "Kross/Renderer/Textures/Textures.h"


namespace Kross::OpenGL::Texture {
	class KAPI Base final : public Kross::Texture::Base
	{
	public:
		static const int QueryMaxSlots();
	};

	class KAPI T2D final : public Kross::Texture::T2D
	{
	public:
		T2D() = default;
		T2D(uint32_t width, uint32_t height, const char* name, unsigned char* data = nullptr);
		T2D(const char* name, const char* path);

		virtual void SetFilter(Filtering_Type filter) override;
		virtual void SetData(unsigned char* data, uint32_t size) override;
		virtual void ResetData() const override;
		virtual const Ref<unsigned char>& GetData() const override;

		virtual ~T2D() override;

		virtual uint32_t GetWidth() const override { return m_unWidth; }
		virtual uint32_t GetHeight() const override { return m_unHeight; }
		virtual glm::vec2 GetSize() const override { return glm::vec2((float)m_unWidth, (float)m_unHeight); }
		virtual uint32_t GetID() const override { return m_RendererID; }
		virtual const char* GetName() const override { return m_strName; }
		virtual int32_t GetCurrentSlot() const override { return m_CurrentSlot; }

		virtual void Bind(uint32_t slot = 0) const override;
	private:
		Ref<unsigned char> raw_data;
		const char* m_strPath, *m_strName;
		uint32_t m_RendererID, m_unWidth, m_unHeight;
		uint8_t m_CurrentSlot;
		int m_unInternalFormat;
		uint32_t m_unDataFormat;
	};
}

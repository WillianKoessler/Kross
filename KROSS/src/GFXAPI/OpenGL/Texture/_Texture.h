#pragma once

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
		T2D(uint32_t width, uint32_t height, const std::string & name, void* data = nullptr);
		T2D(uint32_t width, uint32_t height, void* data = nullptr);
		T2D(const std::string& path, const std::string& name = "Default");

		virtual void SetData(void* data, uint32_t size) override;

		virtual ~T2D() override;

		virtual uint32_t GetWidth() const override { return m_unWidth; }
		virtual uint32_t GetHeight() const override { return m_unHeight; }
		virtual const unsigned int GetID() const override { return m_RendererID; }
		virtual const std::string& GetName() const override { return m_strName; }
		virtual const int GetCurrentSlot() const override { return m_CurrentSlot; }

		virtual void Bind(uint32_t slot = 0) const override;
	private:
		std::string m_strPath, m_strName;
		uint32_t m_RendererID, m_unWidth, m_unHeight;
		unsigned short m_CurrentSlot;
		int m_unInternalFormat;
		unsigned int m_unDataFormat;
	};
}

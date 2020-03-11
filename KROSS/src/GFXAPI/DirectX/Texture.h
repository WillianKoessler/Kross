#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/Textures.h"
#include "DXBasics.h"

namespace Kross::DirectX::Textures {
	class KAPI T2D : public Kross::Textures::T2D
	{
	public:

		T2D(uint32_t width, uint32_t height, void* data = nullptr);
		T2D(const std::string& path);
		virtual void SetData(void* data, uint32_t size) override;

		virtual ~T2D() override;

		virtual uint32_t GetWidth() const override { return m_unWidth; }
		virtual uint32_t GetHeight() const override { return m_unHeight; }


		virtual void Bind(uint32_t slot = 0) const override;
	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pSysBufferTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSysBufferTextureView;
		std::string m_strPath, m_strName;
		int m_unWidth, m_unHeight;
		uint32_t m_RendererID;
		int m_unInternalFormat;
		unsigned int m_unDataFormat;
	};
}

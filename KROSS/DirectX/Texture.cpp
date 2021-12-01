#include <Kross_pch.h>
#include "Texture.h"

#include "Context.h"
#include "stb_image.h"

namespace Kross::DirectX::Textures {
	const char* area = "[Kross::DirectX::Textures]";
	T2D::T2D(uint32_t width, uint32_t height, void* data)
	{
		HRESULT hr;
		D3D11_TEXTURE2D_DESC sysTexDesc;
		sysTexDesc.Width = Context::Get().GetWindow()->GetWidth();
		sysTexDesc.Height = Context::Get().GetWindow()->GetHeight();
		sysTexDesc.MipLevels = 1;
		sysTexDesc.ArraySize = 1;
		sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sysTexDesc.SampleDesc.Count = 1;
		sysTexDesc.SampleDesc.Quality = 0;
		sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
		sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		sysTexDesc.MiscFlags = 0;
		// create the texture
		if (FAILED(hr = Context::Get().GetDevice()->CreateTexture2D(&sysTexDesc, nullptr, &pSysBufferTexture)))
		{
			KROSS_CORE_ERROR("Failed to Create 2D Texture");
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = sysTexDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		// create the resource view on the texture
		if (FAILED(hr = Context::Get().GetDevice()->CreateShaderResourceView(pSysBufferTexture.Get(),
			&srvDesc, &pSysBufferTextureView)))
		{
			KROSS_CORE_ERROR("Failed to Create Resource View on 2D Texture");
		}

	}
	T2D::T2D(const std::string& path)
	{

	}
	void T2D::SetData(void* data, uint32_t size)
	{

	}
	T2D::~T2D()
	{

	}
	void T2D::Bind(uint32_t slot) const
	{

	}
}
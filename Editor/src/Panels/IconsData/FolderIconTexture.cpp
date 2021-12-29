#include "Editor_pch.h"
#include "FolderIconTexture.h"

static uint32_t bytes[] = {
#include "FolderIcon_data.txt"
};

namespace Kross {
	Ref<Texture::T2D> FolderIconTexture::Create()
	{
		return Texture::T2D::CreateRef("FolderIcon", 256, 256, Texture::T2D::DataFormat::UNSIGNED_INT, Texture::T2D::Channels::RGBA8, bytes);
	}
}

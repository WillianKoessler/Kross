#include "Editor_pch.h"
#include "FileIconTexture.h"

static uint32_t bytes[] = {
#include "FileIcon_data.txt"
};

namespace Kross {
	Ref<Texture::T2D> FileIconTexture::Create()
	{
		return Texture::T2D::CreateRef("FileIcon", 256, 256, Texture::T2D::DataFormat::UNSIGNED_INT, Texture::T2D::Channels::RGBA8, bytes);
	}
}

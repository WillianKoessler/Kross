#include "Kross_pch.h"
#include "_Atlas.h"
#include "stb_image.h"
#include "GFXAPI/OpenGL/Context.h"

namespace Kross::OpenGL::Texture {
	T2DAtlas::T2DAtlas(uint32_t width, uint32_t height, void* data)
	{
		//glGenTextures(1, &_ID);
		//glBindTexture(GL_TEXTURE_2D_ARRAY, _ID);
		//// Allocate the storage.
		//glTexStorage2D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGBA8, width, height, layerCount);
		//// Upload pixel data.
		//// The first 0 refers to the mipmap level (level 0, since there's only 1)
		//// The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
		//// The final 0 refers to the layer index offset (we start from index 0 and have 2 levels).
		//// Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
		//glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, layerCount, GL_RGBA, GL_UNSIGNED_BYTE, texels);

		//// Always set reasonable texture parameters
		//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	T2DAtlas::T2DAtlas(uint32_t width, uint32_t height, const std::string& name, void* data)
	{
	}
	T2DAtlas::T2DAtlas(const std::string& path, const std::string& name)
	{
	} 
	uint32_t T2DAtlas::GetWidth() const
	{
		return uint32_t();
	}
	uint32_t T2DAtlas::GetHeight() const
	{
		return uint32_t();
	}
	const std::string& T2DAtlas::GetName() const
	{
		return _name;
	}
	const unsigned int T2DAtlas::GetID() const
	{
		return 0;
	}
	const int T2DAtlas::GetCurrentSlot() const
	{
		return 0;
	}
	void T2DAtlas::SetData(void* data, uint32_t size)
	{
	}
	void T2DAtlas::Bind(uint32_t slot) const
	{
	}
}
#include "Kross_pch.h"
#include "_Atlas.h"
#include "stb_image.h"
#include "GFXAPI/OpenGL/Context.h"

namespace Kross::OpenGL::Texture {
	T2DAtlas::T2DAtlas(uint32_t width, uint32_t height, void* data)
	{
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
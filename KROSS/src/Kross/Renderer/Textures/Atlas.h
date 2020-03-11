#pragma once
#include "Kross/Renderer/Textures/Textures.h"

namespace Kross::Texture {
	class KAPI T2DAtlas : public Base
	{
		static Ref<T2DAtlas> CreateRef(uint32_t width, uint32_t height, void* data = nullptr);
		static Ref<T2DAtlas> CreateRef(uint32_t width, uint32_t height, const std::string& name, void* data = nullptr);
		static Ref<T2DAtlas> CreateRef(const std::string& path, const std::string& name = "");
		static Scope<T2DAtlas> CreateScope(uint32_t width, uint32_t height, void* data = nullptr);
		static Scope<T2DAtlas> CreateScope(uint32_t width, uint32_t height, const std::string& name, void* data = nullptr);
		static Scope<T2DAtlas> CreateScope(const std::string& path, const std::string& name = "");
	};
}
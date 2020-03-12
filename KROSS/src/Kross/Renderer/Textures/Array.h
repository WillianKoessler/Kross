#pragma once
#include "Kross/Renderer/Textures/Textures.h"

namespace Kross::Texture {
	class KAPI Array
	{
	public:
		virtual ~Array() = default;

		virtual void Bind(const size_t slot) const = 0;
		virtual void Add(const Ref<Base> texture) = 0;
		virtual void Del(const Ref<Base> texture) = 0;
		virtual void Del(const size_t index) = 0;

		virtual const size_t size() const = 0;
		virtual const size_t* IDs() const = 0;
	};

	class KAPI T2DArray : public Array
	{
	public:
		virtual const int Get(const Ref<T2D> texture) = 0;
		static Ref<T2DArray> CreateRef(const size_t size);
		static Scope<T2DArray> CreateScope(const size_t size);
	};
}

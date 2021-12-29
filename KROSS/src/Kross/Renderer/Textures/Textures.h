#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Resource.h"
#include <glm/glm.hpp>
#include <cstdint>

namespace Kross::Texture {
	class KAPI Base : public Resource
	{
		static uint32_t texSlotIndex;
	public:
		enum class Filtering_Type { MIN_LINEAR = 0, MAG_LINEAR, MIN_NEAREST, MAG_NEAREST };
		enum class Channels { RGB8, RGB16, RGBA8, RGBA16, };
		enum class DataFormat { UNSIGNED_BYTE, UNSIGNED_INT };

		virtual ~Base() {};

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual Channels GetChannels() const = 0;
		virtual DataFormat GetFormat() const = 0;
		virtual glm::vec2 GetSize() const = 0;
		virtual uint32_t GetID() const = 0;
		virtual int32_t GetCurrentSlot() const = 0;

		virtual void SetFilter(Filtering_Type filtering) = 0;
		virtual void SetData(void *data, uint32_t size, DataFormat fmt, Channels ch) = 0;
		virtual void ResetData() = 0;
		virtual const uint8_t* GetData()  const = 0;
		virtual const char *GetPath() const = 0;
		virtual void DebugPrint(bool reverse) const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		static uint32_t QueryMaxSlots();
		static uint32_t IncSlot();
		static uint32_t GetSlot();
		static void ResetIndex();
	};

	class KAPI T2D : public Base
	{
	public:
		static Ref<T2D> CreateRef(const char* name, uint32_t width, uint32_t height, DataFormat fmt, Channels ch, const void* data = nullptr);
		static Ref<T2D> CreateRef(const char* name, const char* path);
		static Scope<T2D> CreateScope(const char* name, uint32_t width, uint32_t height, DataFormat fmt, Channels ch, const void* data = nullptr);
		static Scope<T2D> CreateScope(const char* name, const char* path);
	};
}

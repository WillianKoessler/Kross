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
		enum Filtering_Type { MIN_LINEAR = 0, MAG_LINEAR, MIN_NEAREST, MAG_NEAREST };
		virtual ~Base() {};

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual glm::vec2 GetSize() const = 0;
		virtual uint32_t GetID() const = 0;
		virtual int32_t GetCurrentSlot() const = 0;

		virtual void SetFilter(Filtering_Type filtering) = 0;
		virtual void SetData(uint8_t* data, uint32_t size) = 0;
		virtual void ResetData() const = 0;
		virtual const Ref<uint8_t>& GetData()  const = 0;
		virtual const char *GetPath() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		static uint32_t QueryMaxSlots();
		static uint32_t IncSlot();
		static uint32_t GetSlot();
		static void ResetIndex();
	};

	class KAPI T2D : public Base
	{
	public:
		static Ref<T2D> CreateRef(const char* name, uint32_t width, uint32_t height, uint8_t* data = nullptr);
		static Ref<T2D> CreateRef(const char* name, const char* path);
		static Scope<T2D> CreateScope(const char* name, uint32_t width, uint32_t height, uint8_t* data = nullptr);
		static Scope<T2D> CreateScope(const char* name, const char* path);
	};
}

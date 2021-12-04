#pragma once

#include "Kross/Core/Core.h"
#include <glm/glm.hpp>
#include <cstdint>

namespace Kross::Texture {
	class KAPI Base
	{
		static unsigned int texSlotIndex;
	public:
		enum Filtering_Type { MIN_LINEAR = 0, MAG_LINEAR, MIN_NEAREST, MAG_NEAREST };
		virtual ~Base() {};

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual glm::vec2 GetSize() const = 0;
		virtual const std::string& GetName() const = 0;
		virtual unsigned int GetID() const = 0;
		virtual int GetCurrentSlot() const = 0;

		virtual void SetFilter(Filtering_Type filtering) = 0;
		virtual void SetData(uint8_t* data, unsigned int size) = 0;
		virtual void ResetData() const = 0;
		virtual const Ref<uint8_t>& GetData()  const = 0;

		virtual void Bind(unsigned int slot = 0) const = 0;

		static unsigned int QueryMaxSlots();
		static unsigned int IncSlot();
		static unsigned int GetSlot();
		static void ResetIndex();
	};

	class KAPI T2D : public Base
	{
	public:
		static Ref<T2D> CreateRef(unsigned int width, unsigned int height, uint8_t* data = nullptr);
		static Ref<T2D> CreateRef(unsigned int width, unsigned int height, const std::string& name, uint8_t* data = nullptr);
		static Ref<T2D> CreateRef(const std::string& name, const std::string& path);
		static Scope<T2D> CreateScope(unsigned int width, unsigned int height, uint8_t* data = nullptr);
		static Scope<T2D> CreateScope(unsigned int width, unsigned int height, const std::string& name, uint8_t* data = nullptr);
		static Scope<T2D> CreateScope(const std::string& name, const std::string& path);
	};
}
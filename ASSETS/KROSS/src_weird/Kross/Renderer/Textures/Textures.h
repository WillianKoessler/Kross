#pragma once

#include "Kross/Core/Core.h"
#include <string>
#include <glm/glm.hpp>

namespace Kross::Texture {
	class KAPI Base
	{
		static unsigned int texSlotIndex;
	public:
		enum Filtering_Type { MIN_LINEAR = 0, MAG_LINEAR, MIN_NEAREST, MAG_NEAREST };
		virtual ~Base() {};

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual glm::vec2 GetSize() const = 0;
		virtual const std::string& GetName() const = 0;
		virtual const unsigned int GetID() const = 0;
		virtual const int GetCurrentSlot() const = 0;

		virtual void SetFilter(Filtering_Type filtering) = 0;
		virtual void SetData(unsigned char* data, uint32_t size) = 0;
		virtual void ResetData() const = 0;
		virtual const Ref<unsigned char>& GetData()  const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		static const unsigned int QueryMaxSlots();
		static const unsigned int IncSlot();
		static const unsigned int GetSlot();
		static void ResetIndex();
	};

	class KAPI T2D : public Base
	{
	public:
		static Ref<T2D> CreateRef(uint32_t width, uint32_t height, unsigned char* data = nullptr);
		static Ref<T2D> CreateRef(uint32_t width, uint32_t height, const std::string& name, unsigned char* data = nullptr);
		static Ref<T2D> CreateRef(const std::string& name, const std::string& path);
		static Scope<T2D> CreateScope(uint32_t width, uint32_t height, unsigned char* data = nullptr);
		static Scope<T2D> CreateScope(uint32_t width, uint32_t height, const std::string& name, unsigned char* data = nullptr);
		static Scope<T2D> CreateScope(const std::string& name, const std::string& path);
	};
}
#pragma once

#include "Kross/Core/Core.h"
#include <string>

namespace Kross::Texture {
	class KAPI Base
	{
		static unsigned int texSlotIndex;
	public:
		virtual ~Base() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const std::string& GetName() const = 0;
		virtual const unsigned int GetID() const = 0;
		virtual const int GetCurrentSlot() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		static const unsigned int QueryMaxSlots();
		static const unsigned int IncSlot();
		static const unsigned int GetSlot();
		static void ResetIndex();
	};

	class KAPI T2D : public Base
	{
	public:
		static Ref<T2D> CreateRef(uint32_t width, uint32_t height, void* data = nullptr);
		static Ref<T2D> CreateRef(uint32_t width, uint32_t height, const std::string& name, void* data = nullptr);
		static Ref<T2D> CreateRef(const std::string& path, const std::string& name = "");
		static Scope<T2D> CreateScope(uint32_t width, uint32_t height, void* data = nullptr);
		static Scope<T2D> CreateScope(uint32_t width, uint32_t height, const std::string& name, void* data = nullptr);
		static Scope<T2D> CreateScope(const std::string& path, const std::string& name = "");
	};
}
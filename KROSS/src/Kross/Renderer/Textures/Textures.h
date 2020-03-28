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
		class proxy
		{
			int mode = -1;
			uint32_t width, height;
			const std::string* name = nullptr, * path = nullptr;
			bool alpha = false;
			void* data = nullptr;
		public:
			proxy(proxy&&) = delete;
			proxy(const proxy&) = delete;
			proxy(uint32_t width, uint32_t height, const std::string& name, bool alpha, void* data = nullptr)
				:
				mode(0),
				width(width),
				height(height),
				name(&name),
				alpha(alpha),
				data(data)
			{}
			proxy(uint32_t width, uint32_t height, bool alpha, void* data = nullptr)
				:
				mode(1),
				width(width),
				height(height),
				alpha(alpha),
				data(data)
			{}
			proxy(uint32_t width, uint32_t height, void* data = nullptr)
				:
				mode(2),
				width(width),
				height(height),
				data(data)
			{}
			proxy(const std::string& path, const std::string& name)
				:
				mode(3),
				name(&name),
				path(&path)
			{}

			operator Ref<T2D>()
			{
				switch (mode)
				{
				case 0:	return T2D::CreateRef(width, height, *name, alpha, data);
				case 1:	return T2D::CreateRef(width, height, alpha, data);
				case 2:	return T2D::CreateRef(width, height, data);
				case 3:	return T2D::CreateRef(*path, *name);
				default:KROSS_MSGBOX("Unknown Constructor", __FUNCTION__, _FATAL_);
					return nullptr;
				}
			}
			operator Scope<T2D>()
			{
				switch (mode)
				{
				case 0:	return T2D::CreateScope(width, height, *name, alpha, data);
				case 1:	return T2D::CreateScope(width, height, alpha, data);
				case 2:	return T2D::CreateScope(width, height, data);
				case 3:	return T2D::CreateScope(*path, *name);
				default:KROSS_MSGBOX("Unknown Constructor", __FUNCTION__, _FATAL_);
					return nullptr;
				}
			}
		};
	public:
		static proxy Create(uint32_t width, uint32_t height, const std::string& name, bool alpha, void* data)
		{
			return proxy(width, height, name, data);
		}
		static proxy Create(uint32_t width, uint32_t height, bool alpha, void* data)
		{
			return proxy(width, height, alpha, data);
		}
		static proxy Create(uint32_t width, uint32_t height, void* data)
		{
			return proxy(width, height, data);
		}
		static proxy Create(const std::string& path, const std::string& name)
		{
			return proxy(path, name);
		}

		static Ref<T2D> CreateRef(uint32_t width, uint32_t height, const std::string& name, bool alpha, void* data);
		static Ref<T2D> CreateRef(uint32_t width, uint32_t height, bool alpha, void* data);
		static Ref<T2D> CreateRef(uint32_t width, uint32_t height, void* data);
		static Ref<T2D> CreateRef(const std::string& path, const std::string& name);
		static Scope<T2D> CreateScope(uint32_t width, uint32_t height, const std::string& name, bool alpha, void* data);
		static Scope<T2D> CreateScope(uint32_t width, uint32_t height, bool alpha, void* data);
		static Scope<T2D> CreateScope(uint32_t width, uint32_t height, void* data);
		static Scope<T2D> CreateScope(const std::string& path, const std::string& name);
	};
}
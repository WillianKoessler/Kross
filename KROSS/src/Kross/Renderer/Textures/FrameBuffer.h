#pragma once
#include "Textures.h"

namespace Kross::Texture {
	class KAPI FrameBuffer
	{
		class proxy
		{
			const std::string& name;
			void* data;
		public:
			proxy(const std::string& name, void* data)
				:
				name(name),
				data(data)
			{}
			operator Ref<FrameBuffer>()
			{
				return FrameBuffer::CreateRef(name, data);
			}
			operator Scope<FrameBuffer>()
			{
				return FrameBuffer::CreateScope(name, data);
			}
		};
		static Ref<FrameBuffer> CreateRef(const std::string& name, void* data);
		static Scope<FrameBuffer> CreateScope(const std::string& name, void* data);
	public:
		static proxy Create(const std::string& name, void* data = nullptr)
		{
			return proxy(name, data);
		}

		virtual void Bind() const = 0;
		virtual void Bind(const uint32_t viewWidth, const uint32_t viewHeight) = 0;
		virtual void unBind() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const std::string& GetName() const = 0;
		virtual const unsigned int GetID() const = 0;
		virtual const Ref<Kross::Texture::T2D> GetTexture() const = 0;

	};
}


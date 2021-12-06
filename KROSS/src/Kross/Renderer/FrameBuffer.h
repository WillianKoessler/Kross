#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Resource.h"
#include <glm/glm.hpp>

namespace Kross {
	/*
	* Width = Width of the Framebuffer
	* Height = Height of the Framebuffer
	* Samples = How many samples the framebuffer is going to have
	* SwapChainTarget = Flag that says if the framebuffer is going to be part of the swapchain
	*/
	struct FrameBufferSpec
	{
		uint32_t Width, Height;
		// FrameBufferFormat format;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class KAPI FrameBuffer : public Resource
	{
	protected:
		FrameBufferSpec specs;
	public:
		virtual void Bind() const = 0;
		virtual void unBind() const = 0;

		virtual void Resize(const glm::uvec2& size) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetID() const = 0;
		virtual uint32_t GetDepthAttachmentID() const = 0;
		virtual uint32_t GetColorAttachmentID() const = 0;

		virtual void Invalidate() = 0;

		virtual const FrameBufferSpec& GetSpecs() const = 0;

		static Ref<FrameBuffer> CreateRef(const char* name, const FrameBufferSpec& specs);
		static Scope<FrameBuffer> CreateScope(const char* name, const FrameBufferSpec& specs);
	};
}
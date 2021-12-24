#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Resource.h"
#include <glm/glm.hpp>

namespace Kross {
	class KAPI FrameBuffer : public Resource
	{
	public:
		struct Texture
		{
			enum class Format
			{
				None = 0,

				//Color Formats
				RGB8,
				RGB16,
				RGB32,
				RGB64,
				RGBA8,
				RGBA16,
				RGBA32,
				RGBA64,

				//Depth/Stencil
				Depth24Stencil8,

				//Default Depth
				Depth = Depth24Stencil8
			};
			struct Spec
			{
				Spec() = default;
				Spec(Format format)
					: m_Format(format) {}

				Format m_Format = Format::None;
				// Filtering / Wrap
			};
		};
		struct AttachSpec
		{
			AttachSpec() = default;
			AttachSpec(const std::initializer_list<Texture::Spec> &attachments)
				: m_Attachments(attachments) {}
			std::vector<Texture::Spec> m_Attachments;
		};
		struct Specification
		{
			AttachSpec AttachmentsSpecs;
			uint32_t Width, Height, Samples;
			bool SwapChainTarget = false;
		};

	protected:
		Specification specs;
	public:
		virtual void Bind() const = 0;
		virtual void unBind() const = 0;

		virtual void Resize(const glm::uvec2 &size) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetID() const = 0;
		virtual uint32_t GetDepthAttachmentID() const = 0;
		virtual uint32_t GetColorAttachmentID(uint32_t index) const = 0;

		virtual void Invalidate() = 0;

		virtual const Specification &GetSpecs() const = 0;

		static Ref<FrameBuffer> CreateRef(const char *name, const Specification &specs);
		static Scope<FrameBuffer> CreateScope(const char *name, const Specification &specs);
	};
}

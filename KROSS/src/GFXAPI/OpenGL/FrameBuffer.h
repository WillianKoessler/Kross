#pragma once

#include "Kross/Renderer/FrameBuffers.h"

namespace Kross::OpenGL
{
	class FrameBuffer : public Kross::FrameBuffers
	{
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FrameBufferSpec m_Specification;
	public:
		FrameBuffer(const FrameBufferSpec& specs);
		virtual ~FrameBuffer();

		virtual void Bind() const override;
		virtual void unBind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		virtual void Invalidate() const override;

		virtual const FrameBufferSpec& GetSpecs() const override { return m_Specification; }
	};
}

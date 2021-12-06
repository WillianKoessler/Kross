#pragma once

#include "Kross/Renderer/FrameBuffer.h"

namespace Kross::OpenGL
{
	class FrameBuffer : public Kross::FrameBuffer
	{
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment, m_DepthAttachment;
		FrameBufferSpec m_Specification;
	public:
		FrameBuffer(const char* name, const FrameBufferSpec& specs);
		virtual ~FrameBuffer();

		virtual void Bind() const override;
		virtual void unBind() const override;

		virtual void Resize(const glm::uvec2& size) override;
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetID() const override { return m_RendererID; }
		virtual uint32_t GetDepthAttachmentID() const override { return m_DepthAttachment; }
		virtual uint32_t GetColorAttachmentID() const override { return m_ColorAttachment; }

		virtual void Invalidate() override;

		virtual const FrameBufferSpec& GetSpecs() const override { return m_Specification; }

	private:
		bool Delete();
	};
}

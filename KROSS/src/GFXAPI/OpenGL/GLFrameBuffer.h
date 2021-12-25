#pragma once

#include "Kross/Renderer/FrameBuffer.h"

namespace Kross::OpenGL
{
	class FrameBuffer : public Kross::FrameBuffer
	{
	public:
		FrameBuffer(const char* name, const Specification & specs);
		virtual ~FrameBuffer();

		virtual void Bind() const override;
		virtual void unBind() const override;

		virtual void Resize(const glm::uvec2& size) override;
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetID() const override { return m_RendererID; }
		virtual uint32_t GetDepthAttachmentID() const override { return m_DepthAttID; }
		virtual uint32_t GetColorAttachmentID(uint32_t index) const override { KROSS_ASSERT(index < m_ColorAttIDs.size()); return m_ColorAttIDs[index]; }
		virtual void ClearColorAttachment(uint32_t index, int value) const override;

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) const override;

		virtual void Invalidate() override;

		virtual const Specification & GetSpecs() const override { return m_Specs; }

	private:
		void Delete();

	private:
		uint32_t m_RendererID;
		uint32_t m_MaxWidth, m_MaxHeight;
		Specification m_Specs;
		
		std::vector<Kross::FrameBuffer::Texture::Spec> m_ColorAttSpecs;
		std::vector<uint32_t> m_ColorAttIDs;

		Kross::FrameBuffer::Texture::Spec m_DepthAttSpec;
		uint32_t m_DepthAttID;
	};
}

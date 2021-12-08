#include "Kross_pch.h"
#include "GLFrameBuffer.h"
#include "glad/glad.h"
#include "GLErrors.h"

namespace Kross::OpenGL {
	FrameBuffer::FrameBuffer(const char *name, const FrameBufferSpec &specs)
		: m_RendererID(0), m_ColorAttachment(0), m_DepthAttachment(0), m_Specification(specs)
	{
		int maxSize[] { 0, 0 };
		glCall(glGetIntegerv(GL_MAX_TEXTURE_SIZE, maxSize));
		m_Specification.MAXWidth = maxSize[0];
		m_Specification.MAXHeight = maxSize[1];
		SetName(name);
		Invalidate();
	}
	FrameBuffer::~FrameBuffer()
	{
		if (!Delete())
			KROSS_CORE_ERROR(
			"Destroying a invalid Framebuffer. '{0}' | IDs: m_RendererID = {1}, m_ColorAttachment = {2}, m_DepthAttachment = {3}",
			GetName(), m_RendererID, m_ColorAttachment, m_DepthAttachment
			);
	}
	void FrameBuffer::Bind() const
	{
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
		glCall(glViewport(0, 0, m_Specification.Width, m_Specification.Height));
	}

	void FrameBuffer::unBind() const
	{
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void FrameBuffer::Resize(const glm::uvec2 &size)
	{
		if (m_Specification.Width == size.x && m_Specification.Height == size.y) return;
		m_Specification.Width = size.x;
		m_Specification.Height = size.y;
		Invalidate();
	}

	void FrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		if (m_Specification.Width == width && m_Specification.Height == height) return;
		if (width == 0 || width > m_Specification.MAXWidth || height == 0 || height > m_Specification.MAXHeight) {
			KROSS_CORE_WARN("Framebuffer set to be Invalid Value. [ width = {0}, height = {1}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}

	void FrameBuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glCall(glDeleteFramebuffers(1, &m_RendererID));
			glCall(glDeleteTextures(1, &m_ColorAttachment));
			glCall(glDeleteTextures(1, &m_DepthAttachment));
		}

		glCall(glGenFramebuffers(1, &m_RendererID));
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

		glCall(glGenTextures(1, &m_ColorAttachment));
		glCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
		glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));

		glCall(glGenTextures(1, &m_DepthAttachment));
		glCall(glBindTexture(GL_TEXTURE_2D, m_DepthAttachment));
		glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
		glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0));

		uint32_t framebufferStatus;
		glCall(framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER));
		switch (framebufferStatus)
		{
			case GL_FRAMEBUFFER_UNDEFINED:						KROSS_CORE_ERROR("The specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist. m_RendererID={0}, m_ColorAttachment={1}, m_DepthAttachment={2}", m_RendererID, m_ColorAttachment, m_DepthAttachment); break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			KROSS_CORE_ERROR("Some of the framebuffer's attachment points are incomplete. m_RendererID={0}, m_ColorAttachment={1}, m_DepthAttachment={2}", m_RendererID, m_ColorAttachment, m_DepthAttachment); break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	KROSS_CORE_ERROR("The framebuffer does not have at least one image attached to it. m_RendererID={0}, m_ColorAttachment={1}, m_DepthAttachment={2}", m_RendererID, m_ColorAttachment, m_DepthAttachment); break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:			KROSS_CORE_ERROR("The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi. m_RendererID={0}, m_ColorAttachment={1}, m_DepthAttachment={2}", m_RendererID, m_ColorAttachment, m_DepthAttachment); break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:			KROSS_CORE_ERROR("GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER. m_RendererID={0}, m_ColorAttachment={1}, m_DepthAttachment={2}", m_RendererID, m_ColorAttachment, m_DepthAttachment); break;
			case GL_FRAMEBUFFER_UNSUPPORTED:					KROSS_CORE_ERROR("The combination of internal formats of the attached images violates an implementation - dependent set of restrictions. m_RendererID={0}, m_ColorAttachment={1}, m_DepthAttachment={2}", m_RendererID, m_ColorAttachment, m_DepthAttachment); break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			KROSS_CORE_ERROR("The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or , if the attached images are a mix of renderbuffersand textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES. Or the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or , if the attached images are a mix of renderbuffersand textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures. m_RendererID={0}, m_ColorAttachment={1}, m_DepthAttachment={2}", m_RendererID, m_ColorAttachment, m_DepthAttachment); break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:		KROSS_CORE_ERROR("One of the framebuffer's attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target. m_RendererID={0}, m_ColorAttachment={1}, m_DepthAttachment={2}", m_RendererID, m_ColorAttachment, m_DepthAttachment); break;
			case GL_FRAMEBUFFER_COMPLETE:						KROSS_CORE_INFO("Framebuffer validated. [ width = {0}, height = {1} ]", m_Specification.Width, m_Specification.Height); break;
			default:											KROSS_CORE_ERROR("Unknown Error. ID={0}", framebufferStatus); break;
		}
		if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE) Delete();
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	bool FrameBuffer::Delete()
	{
		if (m_RendererID)
		{
			glCall(glDeleteFramebuffers(1, &m_RendererID));
			glCall(glDeleteTextures(1, &m_ColorAttachment));
			glCall(glDeleteTextures(1, &m_DepthAttachment));
			return true;
		}
		return false;
	}
}

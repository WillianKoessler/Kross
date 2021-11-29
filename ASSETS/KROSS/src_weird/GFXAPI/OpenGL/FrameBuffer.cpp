#include "Kross_pch.h"
#include "FrameBuffer.h"
#include "glad/glad.h"
#include "GLErrors.h"

namespace Kross::OpenGL {
	FrameBuffer::FrameBuffer(const FrameBufferSpec& specs)
		: m_Specification(specs)
	{
		Invalidate();
	}
	FrameBuffer::~FrameBuffer()
	{
		glCall(glDeleteFramebuffers(1, &m_RendererID));
	}
	void FrameBuffer::Bind() const
	{
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	}

	void FrameBuffer::unBind() const
	{
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void FrameBuffer::Invalidate() const
	{
		glGenFramebuffers(1, (GLuint*)&m_RendererID);
		Bind();

		glCall(glGenTextures(1, (GLuint*)&m_ColorAttachment));
		glCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));

		glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));

		glCall(glGenTextures(1, (GLuint*)&m_DepthAttachment));
		glCall(glBindTexture(GL_TEXTURE_2D, m_DepthAttachment));

		//glCall(glTexStorage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height));
		glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));

		glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0));

		bool framebufferCompletion;
		glCall(framebufferCompletion = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		if (framebufferCompletion) {
			KROSS_MSGBOX("Invalid FrameBuffer.\nglCheckFramebufferStatus(GL_FRAMEBUFFER) found errors.", "Kross::FrameBuffer", _ERROR_);
			return;
		}
		unBind();
	}
}

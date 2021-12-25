#include "Kross_pch.h"
#include "GLFrameBuffer.h"
#include "GLErrors.h"

namespace Util {
	static uint32_t TextureTarget(bool multiSampled)
	{
		return multiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}
	static void CreateTexture(uint32_t count, uint32_t *outID)
	{
		glCall(glGenTextures(count, outID));
	}
	static void BindTexture(bool multiSampled, uint32_t id)
	{
		glCall(glBindTexture(TextureTarget(multiSampled), id));
	}
	static void AttachColorTexture(uint32_t id, int samples, uint32_t internalFormat, uint32_t format, uint32_t width, uint32_t height, int index)
	{
		bool multiSample = samples > 1;
		if (multiSample) {
			glCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE));
		} else {
			glCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		}
		glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multiSample), id, 0));
	}
	static void AttachDepthTexture(uint32_t id, int samples, uint32_t format, uint32_t attachmentType, uint32_t width, uint32_t height)
	{
		bool multiSample = samples > 1;
		if (multiSample) {
			glCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE));
		} else {
			glCall(glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height));
			// TODO: find a way to use glTexImage2D for better support to older devices
			//glCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		}

		glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multiSample), id, 0));
	}
	static bool IsDepthFormat(const Kross::FrameBuffer::Texture::Format &fmt)
	{
		switch (fmt) {
			case Kross::FrameBuffer::Texture::Format::Depth24Stencil8: return true;
		}
		return false;
	}
	static GLenum TextureFormatToGL(Kross::FrameBuffer::Texture::Format fmt)
	{
		switch (fmt) {
			case Kross::FrameBuffer::Texture::Format::RGBA8: return GL_RGBA8;
			case Kross::FrameBuffer::Texture::Format::INT: return GL_RED_INTEGER;
		}
		KROSS_ASSERT(false);
		return 0;
	}
}

namespace Kross::OpenGL {

	FrameBuffer::FrameBuffer(const char *name, const Specification &specs)
		: m_RendererID(0), m_Specs(specs)
	{
		int maxSize[]{ 0, 0 };
		glCall(glGetIntegerv(GL_MAX_TEXTURE_SIZE, maxSize));
		m_MaxWidth = maxSize[0];
		m_MaxHeight = maxSize[1];
		SetName(name);

		for (auto &fmt : m_Specs.AttachmentsSpecs.m_Attachments)
		{
			if (!Util::IsDepthFormat(fmt.m_Format))
				m_ColorAttSpecs.emplace_back(fmt);
			else
				m_DepthAttSpec = fmt;
		}

		Invalidate();
	}
	FrameBuffer::~FrameBuffer()
	{
		Delete();
	}
	void FrameBuffer::Bind() const
	{
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
		glCall(glViewport(0, 0, m_Specs.Width, m_Specs.Height));
	}

	void FrameBuffer::unBind() const
	{
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void FrameBuffer::Resize(const glm::uvec2 &size)
	{
		//if (m_Specs.Width == size.x && m_Specs.Height == size.y) return;
		m_Specs.Width = size.x;
		m_Specs.Height = size.y;
		Invalidate();
	}

	void FrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		if (m_Specs.Width == width && m_Specs.Height == height) return;
		if (width == 0 || width > m_MaxWidth || height == 0 || height > m_MaxHeight) {
			KROSS_WARN("Framebuffer set to be Invalid Value. [ width = {0}, height = {1}", width, height);
			return;
		}
		m_Specs.Width = width;
		m_Specs.Height = height;
		Invalidate();
	}

	void FrameBuffer::ClearColorAttachment(uint32_t index, int value) const
	{
		KROSS_ASSERT(index < m_ColorAttIDs.size());
		auto &spec = m_ColorAttSpecs[index];
		glCall(glClearTexImage(m_ColorAttIDs[index], 0,
			Util::TextureFormatToGL(spec.m_Format), GL_INT, &value));
	}

	int FrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) const
	{
		KROSS_ASSERT(attachmentIndex < m_ColorAttIDs.size());

		glCall(glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex));
		int pixel = -1;
		glCall(glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel));
		return pixel;
	}

	void FrameBuffer::Invalidate()
	{
		Delete();

		glCall(glGenFramebuffers(1, &m_RendererID));
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

		bool multiSample = m_Specs.Samples > 1;
		if (!m_ColorAttSpecs.empty()) {
			if (m_ColorAttIDs.size() != m_ColorAttSpecs.size())
				m_ColorAttIDs.resize(m_ColorAttSpecs.size());
			Util::CreateTexture((uint32_t)m_ColorAttIDs.size(), m_ColorAttIDs.data());
			for (uint32_t i = 0; i < (uint32_t)m_ColorAttIDs.size(); i++) {
				Util::BindTexture(multiSample, m_ColorAttIDs[i]);
				switch (m_ColorAttSpecs[i].m_Format) {
					case Texture::Format::RGBA8:
						Util::AttachColorTexture(m_ColorAttIDs[i], m_Specs.Samples, GL_RGBA8, GL_RGBA, m_Specs.Width, m_Specs.Height, i);
						break;
					case Texture::Format::INT:
						Util::AttachColorTexture(m_ColorAttIDs[i], m_Specs.Samples, GL_R32I ,GL_RED_INTEGER, m_Specs.Width, m_Specs.Height, i);
						break;
				}
			}
		}
		if (m_DepthAttSpec.m_Format != Texture::Format::None) {
			Util::CreateTexture(1, &m_DepthAttID);
			Util::BindTexture(multiSample, m_DepthAttID);
			switch (m_DepthAttSpec.m_Format) {
				case Texture::Format::Depth24Stencil8:
					Util::AttachDepthTexture(m_DepthAttID, m_Specs.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specs.Width, m_Specs.Height);
					break;
			}
		}
		KROSS_ASSERT(m_ColorAttSpecs.size() < 5);
		if (!m_ColorAttSpecs.empty()) {
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glCall(glDrawBuffers((uint32_t)m_ColorAttSpecs.size(), buffers));
		} else {
			glCall(glDrawBuffer(GL_NONE));
		}

		uint32_t framebufferStatus;
		glCall(framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER));
		switch (framebufferStatus)
		{
			case GL_FRAMEBUFFER_UNDEFINED:						KROSS_ERROR("The specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist. '{0}' ({1})", GetName(), m_RendererID); break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			KROSS_ERROR("Some of the framebuffer's attachment points are incomplete. '{0}' ({1})", GetName(), m_RendererID); break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	KROSS_ERROR("The framebuffer does not have at least one image attached to it. '{0}' ({1})", GetName(), m_RendererID); break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:			KROSS_ERROR("The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi. '{0}' ({1})", GetName(), m_RendererID); break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:			KROSS_ERROR("GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER. '{0}' ({1})", GetName(), m_RendererID); break;
			case GL_FRAMEBUFFER_UNSUPPORTED:					KROSS_ERROR("The combination of internal formats of the attached images violates an implementation - dependent set of restrictions. '{0}' ({1})", GetName(), m_RendererID); break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			KROSS_ERROR("The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or , if the attached images are a mix of renderbuffersand textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES. Or the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or , if the attached images are a mix of renderbuffersand textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures. '{0}' ({1})", GetName(), m_RendererID); break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:		KROSS_ERROR("One of the framebuffer's attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target. '{0}' ({1})", GetName(), m_RendererID); break;
			case GL_FRAMEBUFFER_COMPLETE:						break;
			default:											KROSS_ERROR("Unknown Error. ErrorID={2} '{0}' ({1})", GetName(), m_RendererID, framebufferStatus); break;
		}
		if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE) Delete();
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	void FrameBuffer::Delete()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures((uint32_t)m_ColorAttIDs.size(), m_ColorAttIDs.data());
			glDeleteTextures(1, &m_DepthAttID);
			m_DepthAttID = 0;
		}
	}
}

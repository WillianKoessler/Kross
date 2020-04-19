#include "Kross_pch.h"
#include "_FrameBuffer.h"
#include "GFXAPI/OpenGL/GLErrors.h"
#include "GFXAPI/OpenGL/Context.h"
#include "Kross/Core/Application.h"

namespace Kross::OpenGL::Texture {
	FrameBuffer::FrameBuffer(const std::string& name, void* data)
		:
		_name(name),
		_width(Kross::Application::Get().GetWindow().GetWidth()),
		_height(Kross::Application::Get().GetWindow().GetHeight())
	{
		KROSS_PROFILE_FUNC();
		GLerror();
		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		m_RenderTexture = Kross::Texture::T2D::Create(_width, _height, name + " | FrameBufferTexture", false, nullptr);

		glGenRenderbuffers(1, &m_DepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderTexture->GetID(), 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			KROSS_MSGBOX("Unexpected Error during FrameBuffer Creation", __FUNCTION__, _ERROR_);
		else
			KROSS_CORE_INFO("[ {0} ] |||| FrameBuffer '{1}' Created", __FUNCTION__, _name);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	FrameBuffer::~FrameBuffer()
	{
		GLerror();
		glDeleteFramebuffers(1, &m_RendererID);
		KROSS_CORE_INFO("[ {0} ] |||| FrameBuffer '{1}' Destructed", __FUNCTION__, _name);
	}
	void FrameBuffer::Bind() const
	{
		GLerror();
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, _width, _height);
		glEnable(GL_DEPTH_TEST);
	}
	void FrameBuffer::Bind(const uint32_t viewWidth, const uint32_t viewHeight)
	{
		GLerror();
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, _width = viewWidth, _height = viewHeight);
		glEnable(GL_DEPTH_TEST);
	}
	void FrameBuffer::unBind() const
	{
		GLerror();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glViewport(100, 100, 400, 200);
		glDisable(GL_DEPTH_TEST);
		glViewport(0, 0,
			Kross::Application::Get().GetWindow().GetWidth(),
			Kross::Application::Get().GetWindow().GetHeight());
	}
}
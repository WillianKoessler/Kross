#include <Kross_pch.h>
#include "FrameBuffer.h"

#include "GLErrors.h"

namespace Kross::OpenGL {
	FrameBuffer::FrameBuffer()
	{
		glCall(glGenFramebuffers(1, &m_RendererID));
		glCall(glBindFrameBuffer(GL_FRAMEBUFFER, m_RendererID));
	}
}
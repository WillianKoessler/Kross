#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/FrameBuffers.h"

namespace Kross::OpenGL {
	class KAPI FrameBuffer : public Kross::FrameBuffers
	{
		FrameBuffer();
	private:
		uint32_t m_RendererID;
	};
}
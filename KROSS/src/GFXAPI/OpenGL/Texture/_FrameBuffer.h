#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/Textures/FrameBuffer.h"
#include "Kross/Renderer/Textures/Textures.h"

namespace Kross::OpenGL::Texture {
	class KAPI FrameBuffer final : public Kross::Texture::FrameBuffer
	{
	public:
		FrameBuffer(const std::string & name, void* data = nullptr);
		~FrameBuffer();

		virtual void Bind() const override;
		virtual void Bind(const uint32_t viewWidth, const uint32_t viewHeight) override;
		virtual void unBind() const override;

		virtual uint32_t GetWidth() const override { return _width; }
		virtual uint32_t GetHeight() const override { return _height; }
		virtual const std::string& GetName() const override { return _name; }
		virtual const unsigned int GetID() const override { return m_RendererID; }
		virtual const Ref<Kross::Texture::T2D> GetTexture() const override { return m_RenderTexture; }

	private:
		std::string _name;
		uint32_t m_RendererID, m_DepthBuffer, m_DrawList[1];
		uint16_t _width, _height;
		Ref<Kross::Texture::T2D> m_RenderTexture;
	};
}


#include "Kross_pch.h"
#include "_Texture.h"
#include "stb_image.h"
#include "GFXAPI/OpenGL/Context.h"

namespace Kross::OpenGL::Texture {
	const int Base::QueryMaxSlots()
	{
		static int query = -1;
		if (query != -1) return query;
		glCall(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &query));
		return query;
	}

	T2D::T2D(uint32_t width, uint32_t height, const std::string& name, void* data)
		:
		m_strName(name),
		m_unWidth(width),
		m_unHeight(height),
		m_CurrentSlot(IncSlot()),
		m_unInternalFormat(GL_RGBA8),
		m_unDataFormat(GL_RGBA)
	{
		KROSS_PROFILE_FUNC();
		if (Context::GetVersion() < 4.5)
		{
			glCall(glGenTextures(1, &m_RendererID));
			glCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		}
		else
		{
			glCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));

			glCall(glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_unWidth, m_unHeight));
			glCall(glTextureStorage2D(m_RendererID, 1, GL_RGB16, m_unWidth, m_unHeight));

			glCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			glCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		}

		if (data) SetData(data, m_unWidth * m_unHeight * (m_unDataFormat == GL_RGBA ? 4 : 3));
		KROSS_CORE_INFO("[Kross::OpenGL::Texture::T2D] Texture '{0}' Created", m_strName);
	}
	T2D::T2D(uint32_t width, uint32_t height, void* data)
		: m_unWidth(width),
		m_unHeight(height),
		m_CurrentSlot(IncSlot()),
		m_unInternalFormat(GL_RGBA8),
		m_unDataFormat(GL_RGBA)
	{
		KROSS_PROFILE_FUNC();
		if (Context::GetVersion() < 4.5f)
		{
			glCall(glGenTextures(1, &m_RendererID));
			glCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		}
		else
		{	 //------------OpenGL 4.5------------
			glCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));

			glCall(glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_unWidth, m_unHeight));
			glCall(glTextureStorage2D(m_RendererID, 1, GL_RGB16, m_unWidth, m_unHeight));

			glCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			glCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		}

		if (data) SetData(data, m_unWidth * m_unHeight * (m_unDataFormat == GL_RGBA ? 4 : 3));
		KROSS_CORE_INFO("[Kross::OpenGL::Texture::T2D] Texture '{0}' Created", m_strName);
	}
	T2D::T2D(const std::string& name, const std::string& path)
		:
		m_strPath(path),
		m_strName(name),
		m_unDataFormat(0),
		m_unInternalFormat(0),
		m_CurrentSlot(IncSlot())
	{
		KROSS_PROFILE_FUNC();

		if (name != "")
		{
			m_strName = FileName(path);
		}

		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		stbi_uc* data = nullptr;
		{
			KROSS_PROFILE_SCOPE("T2D::T2D(const std::string&) - stbi_load");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		if (!data) { KROSS_MSGBOX("Failed to load image!\nFILE: " + path, "[Kross::OpenGL::Texture::T2D]", _ERROR_); }

		m_unWidth = width;
		m_unHeight = height;

		if (channels == 3)
		{
			m_unDataFormat = GL_RGB;
			m_unInternalFormat = GL_RGB8;
		}
		else if (channels == 4)
		{
			m_unDataFormat = GL_RGBA;
			m_unInternalFormat = GL_RGBA8;
		}
		else
		{
			KROSS_CORE_WARN("[Kross::OpenGL::Texture::T2D] Image format not suported!\nFILE: {0}", path);
		}

		if (Context::GetVersion() < 4.5f)
		{
			glCall(glGenTextures(1, &m_RendererID));
			glCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

			if(data) glCall(glTexImage2D(GL_TEXTURE_2D, 0, m_unInternalFormat, m_unWidth, m_unHeight, 0, m_unDataFormat, GL_UNSIGNED_BYTE, (const void*)data));
		}
		else
		{	 //------------OpenGL 4.5------------
			glCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));

			glCall(glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_unWidth, m_unHeight));
			glCall(glTextureStorage2D(m_RendererID, 1, GL_RGB16, m_unWidth, m_unHeight));

			glCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			glCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

			glCall(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_unWidth, m_unHeight, GL_RGB, GL_UNSIGNED_BYTE, data));
		}

		stbi_image_free(data);
		KROSS_CORE_INFO("[Kross::OpenGL::Texture::T2D] Texture '{0}' Created", m_strName);
	}
	T2D::~T2D()
	{
		KROSS_PROFILE_FUNC();
		glCall(glDeleteTextures(1, &m_RendererID));
		KROSS_CORE_INFO("[Kross::OpenGL::Texture::T2D] Texture '{0}' Destructed", m_strName);
	}
	void T2D::SetData(void* data, uint32_t size)
	{
		KROSS_PROFILE_FUNC();
		//if (size != m_unWidth * m_unHeight * (m_unDataFormat == GL_RGBA ? 4 : 3))
		//{
		//	KROSS_CORE_WARN("[Kross::OpenGL::Texture::T2D] Specified size does not match data size.");
		//}
		Bind();
		glCall(glTexImage2D(GL_TEXTURE_2D, 0, m_unInternalFormat, m_unWidth, m_unHeight, 0, m_unDataFormat, GL_UNSIGNED_BYTE, (const void*)data));
	}
	void T2D::Bind(uint32_t slot) const
	{
		KROSS_PROFILE_FUNC();
		if (m_RendererID)
		{
			if (Context::GetVersion() < 4.4)
			{
				if (slot) { glCall(glActiveTexture(GL_TEXTURE0 + slot)); }
				else { glCall(glActiveTexture(GL_TEXTURE0 + m_CurrentSlot)); }
				glCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
			}
			else if (Context::GetVersion() < 4.5)
			{
				if (slot) { glCall(glActiveTexture(GL_TEXTURE0 + slot)); }
				else { glCall(glActiveTexture(GL_TEXTURE0 + m_CurrentSlot)); }
				glCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
			}
			else
			{
				if (slot) { glCall(glBindTextureUnit(slot, m_RendererID)); }
				else { glCall(glBindTextureUnit(m_CurrentSlot, m_RendererID)); }
			}
		}
	}
}
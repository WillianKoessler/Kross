#include "Kross_pch.h"
#include "GLTexture.h"
#include "stb_image.h"
#include "GFXAPI/OpenGL/GLContext.h"
#include "Kross/Util/Util.h"

namespace Kross::OpenGL::Texture {
	const int Base::QueryMaxSlots()
	{
		static int query = -1;
		if (query != -1) return query;
		glCall(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &query));
		return query;
	}

	T2D::T2D(const char *name, uint32_t width, uint32_t height, uint8_t *data)
		: m_strPath(""), m_unWidth(width), m_unHeight(height), m_CurrentSlot(IncSlot()), m_unInternalFormat(GL_RGBA8), m_unDataFormat(GL_RGBA)
	{
		SetName(name);
		if (Context::GetVersion() < 4.5)
		{
			glCall(glGenTextures(1, &m_RendererID));
			glCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		} else
		{
			glCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));

			glCall(glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_unWidth, m_unHeight));
			glCall(glTextureStorage2D(m_RendererID, 1, GL_RGB16, m_unWidth, m_unHeight));

			glCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			glCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		}

		if (data) SetData(data, m_unWidth * m_unHeight * (m_unDataFormat == GL_RGBA ? 4 : 3));
		else KROSS_CORE_WARN("Texture '{0}' NOT created. There were no data.", GetName());
		KROSS_CORE_INFO("'{0}' Constructed", GetName());
	}
	T2D::T2D(const char *name, const char* path)
		: m_strPath(path), m_unDataFormat(0), m_unInternalFormat(0), m_CurrentSlot(IncSlot())
	{
		if (!strcmp(name, "Unnamed_Resource") || !strcmp(name, "")) SetName(FileName(path));
		else SetName(name);

		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		{
			KROSS_PROFILE_SCOPE("T2D::T2D - stbi_load");
			raw_data.reset(stbi_load(path, &width, &height, &channels, 0));
		}
		if (!raw_data) { KROSS_MSGBOX_ERROR("Failed to load image!\nFILE: {0}", path); }

		m_unWidth = width;
		m_unHeight = height;

		if (channels == 3)
		{
			m_unDataFormat = GL_RGB;
			m_unInternalFormat = GL_RGB8;
		} else if (channels == 4)
		{
			m_unDataFormat = GL_RGBA;
			m_unInternalFormat = GL_RGBA8;
		} else
			KROSS_CORE_WARN("Image format not suported!\nFILE: {0}", path);

		if (Context::GetVersion() < 4.5f)
		{
			glCall(glGenTextures(1, &m_RendererID));
			glCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

			if (raw_data) glCall(glTexImage2D(GL_TEXTURE_2D, 0, m_unInternalFormat, m_unWidth, m_unHeight, 0, m_unDataFormat, GL_UNSIGNED_BYTE, (const void *)raw_data.get()));
		} else
		{	 //------------OpenGL 4.5------------
			glCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));

			glCall(glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_unWidth, m_unHeight));
			glCall(glTextureStorage2D(m_RendererID, 1, GL_RGB16, m_unWidth, m_unHeight));

			glCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			glCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

			glCall(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_unWidth, m_unHeight, GL_RGB, GL_UNSIGNED_BYTE, raw_data.get()));
		}

		//stbi_image_free(data);
		KROSS_CORE_INFO("'{0}' Constructed", GetName());
	}
	void T2D::SetFilter(Filtering_Type filter)
	{
		Bind();
		if (Context::GetVersion() < 4.5f) {
			switch (filter)
			{
				case MIN_LINEAR: glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); break;
				case MAG_LINEAR: glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); break;
				case MIN_NEAREST: glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)); break;
				case MAG_NEAREST: glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)); break;
				default: KROSS_CORE_ERROR("Invalid Filtering Type. '{1}'", std::to_string(filter));
			}
		} else {
			switch (filter)
			{
				case MIN_LINEAR: glCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); break;
				case MAG_LINEAR: glCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); break;
				case MIN_NEAREST: glCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST)); break;
				case MAG_NEAREST: glCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST)); break;
				default: KROSS_CORE_ERROR("Invalid Filtering Type. '{1}'", std::to_string(filter));
			}
		}
	}
	T2D::~T2D()
	{
		glCall(glDeleteTextures(1, &m_RendererID));
	}
	void T2D::SetData(uint8_t *data, uint32_t size)
	{
		KROSS_PROFILE_FUNC();
		if (data)
		{
			if (size != m_unWidth * m_unHeight * (m_unDataFormat == GL_RGBA ? 4 : 3))
				KROSS_CORE_WARN("Texture '{0}' Specified size does not match data size.", GetName());
			Bind();
			glCall(glTexImage2D(GL_TEXTURE_2D, 0, m_unInternalFormat, m_unWidth, m_unHeight, 0, m_unDataFormat, GL_UNSIGNED_BYTE, (const void *)data));
			raw_data.reset(data);
		}
	}
	const Ref<unsigned char> &T2D::GetData() const
	{
		return raw_data;
	}
	void T2D::ResetData() const
	{
		if (raw_data)
		{
			Bind();
			glCall(glTexImage2D(GL_TEXTURE_2D, 0, m_unInternalFormat, m_unWidth, m_unHeight, 0, m_unDataFormat, GL_UNSIGNED_BYTE, (const void *)raw_data.get()));
		}
	}
	void T2D::Bind(uint32_t slot) const
	{
		KROSS_PROFILE_FUNC();
		if (m_RendererID)
		{
			if (Context::GetVersion() < 4.5)
			{
				if (slot) { glCall(glActiveTexture(GL_TEXTURE0 + slot)); } else { glCall(glActiveTexture(GL_TEXTURE0 + m_CurrentSlot)); }
				glCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
			} else {
				if (slot) { glCall(glBindTextureUnit(slot, m_RendererID)); } else { glCall(glBindTextureUnit(m_CurrentSlot, m_RendererID)); }
			}
		}
	}
}
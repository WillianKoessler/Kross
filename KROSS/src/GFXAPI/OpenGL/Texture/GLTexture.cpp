#include "Kross_pch.h"
#include "GLTexture.h"
#include "stb_image.h"
#include "GFXAPI/OpenGL/GLContext.h"
#include "Kross/Util/Util.h"

namespace Util {
	static void CreateTexture(uint32_t *id, uint32_t internalFormat, uint32_t format, uint32_t width, uint32_t height)
	{
		glCall(glGenTextures(1, id));
		glCall(glBindTexture(GL_TEXTURE_2D, *id));

		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	}
	static GLenum DataFormatToGL(Kross::Texture::T2D::DataFormat fmt)
	{
		switch (fmt)
		{
			case Kross::Texture::T2D::DataFormat::UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
			case Kross::Texture::T2D::DataFormat::UNSIGNED_INT: return GL_UNSIGNED_INT_8_8_8_8;
		}
		KROSS_ASSERT(false, "Unknown Data Format");
	}
	static uint8_t ChannelsToGL(Kross::Texture::T2D::Channels ch, uint32_t &inter, uint32_t &outCh)
	{
		switch (ch) {
			case Kross::Texture::T2D::Channels::RGB8:
				outCh = GL_RGB;
				inter = GL_RGB8;
				return 3;
			case Kross::Texture::T2D::Channels::RGBA8:
				outCh = GL_RGBA;
				inter = GL_RGBA8;
				return 4;
		}
		KROSS_ASSERT(false, "Unknown Channels Format");
		return 0;
	}
}

namespace Kross::OpenGL::Texture {
	const int Base::QueryMaxSlots()
	{
		static int query = -1;
		if (query != -1) return query;
		glCall(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &query));
		return query;
	}

	T2D::T2D(const char *name, uint32_t width, uint32_t height, DataFormat fmt, Channels ch, const void *data)
		: m_unWidth(width), m_unHeight(height), m_CurrentSlot(IncSlot()), m_unInternalFormat(0), m_unChannels(0)
	{
		SetName(name);


		m_unDataFormat = Util::DataFormatToGL(fmt);
		auto size = width * height * Util::ChannelsToGL(ch, m_unInternalFormat, m_unChannels);
		raw_data = new uint8_t[size];
		memcpy(raw_data, data, size);

		Util::CreateTexture(&m_RendererID, m_unInternalFormat, m_unChannels, m_unWidth, m_unHeight);
		glCall(glTexImage2D(GL_TEXTURE_2D, 0, m_unInternalFormat, m_unWidth, m_unHeight, 0, m_unChannels, m_unDataFormat, raw_data));

		if (!data) KROSS_WARN("Texture '{0}' created with no data.", GetName());
		KROSS_INFO("'{0}' Constructed", GetName());
	}
	T2D::T2D(const char *name, const char *path)
		: m_strPath(path), m_unChannels(0), m_unInternalFormat(0), m_CurrentSlot(IncSlot())
	{
		if (!strcmp(name, "Unnamed_Resource") || !strcmp(name, "")) SetName(FileName(path));
		else SetName(name);

		stbi_set_flip_vertically_on_load(true);

		int width, height, _channels;
		{
			KROSS_PROFILE_SCOPE("T2D::T2D - stbi_load");
			raw_data = stbi_load(path, &width, &height, &_channels, 0);
		}
		if (!raw_data) { KROSS_MSGBOX_ERROR("Failed to load image!\nFILE: {0}", path); }

		m_unWidth = width;
		m_unHeight = height;

		if (_channels == 3) channels = Channels::RGB8;
		else if (_channels == 4) channels = Channels::RGBA8;
		else KROSS_WARN("Image format not suported!\nFILE: {0}", path);

		m_unDataFormat = Util::DataFormatToGL(DataFormat::UNSIGNED_BYTE);
		Util::ChannelsToGL(channels, m_unInternalFormat, m_unChannels);
		Util::CreateTexture(&m_RendererID, m_unInternalFormat, m_unChannels, m_unWidth, m_unHeight);

		glCall(glTexImage2D(GL_TEXTURE_2D, 0, m_unInternalFormat, m_unWidth, m_unHeight, 0, m_unChannels, m_unDataFormat, (const void *)raw_data));

		KROSS_INFO("'{0}' Constructed", GetName());
	}
	void T2D::SetFilter(Filtering_Type filter)
	{
		Bind();
		switch (filter)
		{
			case Filtering_Type::MIN_LINEAR: glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); break;
			case Filtering_Type::MAG_LINEAR: glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); break;
			case Filtering_Type::MIN_NEAREST: glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)); break;
			case Filtering_Type::MAG_NEAREST: glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)); break;
			default: KROSS_ERROR("Invalid Filtering Type. '{1}'", std::to_string((int)filter));
		}
	}
	T2D::~T2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}
	void T2D::SetData(void *data, uint32_t size, DataFormat fmt, Channels ch)
	{
		KROSS_PROFILE_FUNC();
		if (data)
		{
			Util::ChannelsToGL(ch, m_unInternalFormat, m_unChannels);
			m_unDataFormat = Util::DataFormatToGL(fmt);
			auto chs = 0lu;
			if (ch == Channels::RGB8) chs = 3;
			else if (ch == Channels::RGBA8) chs = 4;
			else {
				KROSS_WARN("Invalid Channel Format");
				return;
			}

			if (fmt == DataFormat::UNSIGNED_BYTE) {
				//if(raw_data) delete raw_data;
				memcpy_s(raw_data, (uint64_t)m_unWidth * (uint64_t)m_unHeight * (uint64_t)chs, data, size);
			} else if (fmt == DataFormat::UNSIGNED_INT) {
				uint8_t *toData = new uint8_t[size * chs];
				uint32_t *idata = reinterpret_cast<uint32_t *>(data);
				for (auto i = 0u; i < size; i++) {
					toData[i + 0] = (uint8_t)((idata[i] & 0xFF000000) >> 24);
					toData[i + 1] = (uint8_t)((idata[i] & 0x00FF0000) >> 16);
					toData[i + 2] = (uint8_t)((idata[i] & 0x0000FF00) >> 8);
					toData[i + 3] = (uint8_t)((idata[i] & 0x000000FF) >> 0);
				}
				memcpy_s(raw_data, (uint64_t)m_unWidth * (uint64_t)m_unHeight * (uint64_t)chs, toData, (uint64_t)size * chs);
				delete[] toData;
			} else {
				KROSS_WARN("Invalid Data Format");
				return;
			}

			Bind();
			glCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_unWidth, m_unHeight, m_unChannels, m_unDataFormat, data));
		}
	}
	const uint8_t *T2D::GetData() const
	{
		return raw_data;
	}
	void T2D::DebugPrint(bool reverse) const
	{
		if (channels != Channels::RGBA8) KROSS_ERROR("Channel Type not supported for printing. '{0}'", GetName());
		//if (format != DataFormat::UNSIGNED_INT) KROSS_ERROR("Data Format not supported for printing. '{0}'", GetName());
		std::ofstream of(std::string(GetName()) + "_data.txt", std::ofstream::trunc);
		uint32_t *data = (uint32_t *)raw_data;
		for (uint32_t i = 0; i < m_unWidth * m_unHeight; i++) {
			uint32_t newInt = 0;
			if (reverse) newInt =
				(data[i] & 0x000000FF) << (3 * 8) |
				(data[i] & 0x0000FF00) << (1 * 8) |
				(data[i] & 0x00FF0000) >> (1 * 8) |
				(data[i] & 0xFF000000) >> (3 * 8);
			else newInt = data[i];
			std::stringstream ss;
			ss << std::setfill('0') << std::setw(sizeof(uint32_t) * 2) << std::hex << newInt;
			of << "0x" << ss.str() << "," << ((i + 1) % 20 == 0 ? "\n" : " ");
		}
		of.close();
	}
	void T2D::ResetData()
	{
		Bind();
		glCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_unWidth, m_unHeight, m_unChannels, GL_UNSIGNED_BYTE, nullptr));
		delete raw_data;
	}
	void T2D::Bind(uint32_t slot) const
	{
		KROSS_PROFILE_FUNC();
		if (m_RendererID)
		{
			if (slot) { glCall(glActiveTexture(GL_TEXTURE0 + slot)); } else { glCall(glActiveTexture(GL_TEXTURE0 + m_CurrentSlot)); }
			glCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
		}
	}
}

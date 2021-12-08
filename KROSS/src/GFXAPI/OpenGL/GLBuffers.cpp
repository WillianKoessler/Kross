#include "Kross_pch.h"
#include "GLBuffers.h"
#include "GLErrors.h"

namespace Kross::OpenGL::Buffer {

	Vertex::Vertex(const char *name, float *vertices, uint32_t size, bool dynamic)
		: _size(size),
		dynamic(dynamic)
	{
		KROSS_PROFILE_FUNC();
		if (!vertices) { KROSS_INFO("Creating Vertex Buffer Object with nullptr data"); }
		glCall(glGenBuffers(1, &m_RendererID));
		glCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		glCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
		glCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		SetName(name);
		KROSS_INFO("'{0}' Constructed", GetName());
	}
	Vertex::~Vertex()
	{
		glCall(glDeleteBuffers(1, &m_RendererID));
	}
	void Vertex::Bind() const
	{
		glCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}
	void Vertex::unBind() const
	{
		glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
	void Vertex::upload(const void *data, size_t s) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			Bind();
			glCall(glBufferSubData(GL_ARRAY_BUFFER, 0, (s > _size ? _size : s), data));
		}
	}
	void Vertex::SetLayout(const Kross::Buffer::Layout &layout)
	{
		m_Layout = layout;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Index::Index(const char *name, uint32_t *indices, uint32_t count)
		: m_Count(count / sizeof(uint32_t))
	{
		KROSS_PROFILE_FUNC();
		if (!indices) { KROSS_INFO("Creating buffer with nullptr data"); }
		glCall(glGenBuffers(1, &m_RendererID));
		glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indices, GL_STATIC_DRAW));
		SetName(name);
		KROSS_INFO("'{0}' Constructed", GetName());
	}
	Index::~Index()
	{
		glCall(glDeleteBuffers(1, &m_RendererID));
	}
	void Index::Bind() const
	{
		glCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}
	void Index::unBind() const
	{
		glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

}

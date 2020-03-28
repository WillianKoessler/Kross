#include "Kross_pch.h"
#include "Buffers.h"
#include "GLErrors.h"

namespace Kross::OpenGL::Buffer {
	Vertex::Vertex(float* vertices, uint32_t size, bool dynamic)
		: _size(size),
		dynamic(dynamic)
	{
		KROSS_PROFILE_FUNC();
		GLerror();
		if (!vertices) { KROSS_CORE_WARN("[ {0} ] |||| Creating Vertex Buffer Object with nullptr data", __FUNCTION__); }
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	Vertex::~Vertex()
	{
		KROSS_PROFILE_FUNC();
		GLerror();
		glDeleteBuffers(1, &m_RendererID);
	}
	void Vertex::Bind() const
	{
		KROSS_PROFILE_FUNC();
		GLerror();
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	void Vertex::unBind() const
	{
		KROSS_PROFILE_FUNC();
		GLerror();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void Vertex::upload(const void* data, size_t s) const
	{
		GLerror();
		if (m_RendererID != UINT32_MAX)
		{
			Bind();
			glBufferSubData(GL_ARRAY_BUFFER, 0, (s > _size ? _size : s), data);
		}
	}
	void Vertex::SetLayout(const Kross::Buffer::Layout& layout)
	{
		m_Layout = layout;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Index::Index(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		KROSS_PROFILE_FUNC();
		GLerror();
		if (!indices) { KROSS_CORE_ERROR("[ {0} ] |||| Creating buffer with nullptr data", __FUNCTION__); }
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	Index::~Index()
	{
		KROSS_PROFILE_FUNC();
		GLerror();
		glDeleteBuffers(1, &m_RendererID);
	}
	void Index::Bind() const
	{
		KROSS_PROFILE_FUNC();
		GLerror();
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	void Index::unBind() const
	{
		KROSS_PROFILE_FUNC();
		GLerror();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}

#include "Kross_pch.h"
#include "VertexArrays.h"
#include "GLErrors.h"

namespace Kross::OpenGL {

	unsigned int convertion_to_GLTYPE(const Kross::Buffer::ShaderDataType type)
	{
		if		(type == Kross::Buffer::ShaderDataType::Float)	return GL_FLOAT;
		else if (type == Kross::Buffer::ShaderDataType::Float2)	return GL_FLOAT;
		else if (type == Kross::Buffer::ShaderDataType::Float3)	return GL_FLOAT;
		else if (type == Kross::Buffer::ShaderDataType::Float4)	return GL_FLOAT;
		else if (type == Kross::Buffer::ShaderDataType::Mat3)	return GL_FLOAT;
		else if (type == Kross::Buffer::ShaderDataType::Mat4)	return GL_FLOAT;
		else if (type == Kross::Buffer::ShaderDataType::Int)	return GL_INT;
		else if (type == Kross::Buffer::ShaderDataType::Int2)	return GL_INT;
		else if (type == Kross::Buffer::ShaderDataType::Int3)	return GL_INT;
		else if (type == Kross::Buffer::ShaderDataType::Int4)	return GL_INT;
		else if (type == Kross::Buffer::ShaderDataType::Bool)	return GL_BOOL;
		else if (type == Kross::Buffer::ShaderDataType::Bool2)	return GL_UNSIGNED_BYTE;
		return NULL;
	}

	VertexArray::VertexArray()
	{
		KROSS_PROFILE_FUNC();
		glCall(glGenVertexArrays(1, &m_RendererID));
		glBindVertexArray(m_RendererID);
		KROSS_CORE_INFO("[ {0} ] |||| Vertex Array Created", __FUNCTION__);
	}

	VertexArray::~VertexArray()
	{
		KROSS_PROFILE_FUNC();
		glCall(glDeleteVertexArrays(1, &m_RendererID));
		KROSS_CORE_INFO("[ {0} ] |||| Vertex Array Deleted", __FUNCTION__);
	}

	void VertexArray::Bind() const
	{
		KROSS_PROFILE_FUNC();
		glCall(glBindVertexArray(m_RendererID));
	}

	void VertexArray::unBind() const
	{
		KROSS_PROFILE_FUNC();
		glCall(glBindVertexArray(0));
	}

	void VertexArray::AddVertex(const Ref<Kross::Buffer::Vertex>& vertex)
	{
		KROSS_PROFILE_FUNC();
		this->Bind();
		vertex->Bind();

		if(vertex->GetLayout().GetElements().size() == 0)
			KROSS_CORE_WARN("[ {0} ] Vertex Buffer has no layout", __FUNCTION__);
		
		uint32_t index = 0;
		Buffer::Layout layout = vertex->GetLayout();
		for (const Buffer::Element& element : layout)
		{
			glCall(glEnableVertexAttribArray(index));
			glCall(glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				convertion_to_GLTYPE(element.Type),
				element.Normalized,
				layout.GetStride(),
				(const void*)element.OffSet
			));
			index++;
		}

		m_vecVertex.push_back(vertex);
		KROSS_CORE_TRACE("[ {0} ] |||| Vertex Buffer Added", __FUNCTION__);
	}

	void VertexArray::SetIndex(const Ref<Kross::Buffer::Index>& index)
	{
		KROSS_PROFILE_FUNC();
		this->Bind();
		index->Bind();
		m_Index = index;
		KROSS_CORE_TRACE("[ {0} ] |||| Index Buffer Setted", __FUNCTION__);
	}


}
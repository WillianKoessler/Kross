#include "Kross_pch.h"
#include "Buffers.h"
#include "Renderer.h"

#include "GFXAPI/OpenGL/Buffer.h"


namespace Kross::Buffer {

	Layout::Layout(const std::initializer_list<Element>& elements)
		: m_Elements(elements)
	{
		uint32_t offset = 0;
		m_Stride = 0;
		for (Element& element : m_Elements)
		{
			element.OffSet = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

	void Layout::CalcOffnStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;
		for (Element& element : m_Elements)
		{
			element.OffSet = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Ref<Vertex> Vertex::Create(float* vertices, uint32_t size, bool dynamic)
	{
		KROSS_CORE_INFO("[{0}] Creating VertexBufferObject...", __FUNCTION__);
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__ , _ERROR_); return nullptr;
		case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Buffer::Vertex>(vertices, size, dynamic);
		}
		
		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_);
		return nullptr;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Ref<Index> Index::Create(uint32_t* indices, uint32_t size)
	{
		KROSS_CORE_INFO("[{0}] Creating IndexBufferObject...", __FUNCTION__);
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _FATAL_);return nullptr;
		case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Buffer::Index>(indices, size);
		}

		KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _FATAL_);
		return nullptr;
	}

}

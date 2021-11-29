#include "Kross_pch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "GFXAPI/OpenGL/Buffers.h"


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
		KROSS_CORE_INFO("{0} Creating VertexBufferObject...", __FUNCTION__);
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		KROSS_CORE_WARN("Renderer API (None) is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Buffer::Vertex>(vertices, size, dynamic);
		}

		KROSS_CORE_WARN("Renderer API (None) is not supported");
		return nullptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Ref<Index> Index::Create(uint32_t* indices, uint32_t size)
	{
		KROSS_CORE_INFO("{0} Creating IndexBufferObject...", __FUNCTION__);
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		KROSS_CORE_WARN("Renderer API (None) is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Buffer::Index>(indices, size);
		}

		KROSS_CORE_WARN("Renderer API (None) is not supported");
		return nullptr;
	}

}

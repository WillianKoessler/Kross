#include "Kross_pch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "GFXAPI/OpenGL/GLBuffers.h"


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

	Ref<Vertex> Vertex::Create(const char* name, float* vertices, uint32_t size, bool dynamic)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		KROSS_CORE_WARN("Renderer API (None) is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Buffer::Vertex>(name, vertices, size, dynamic);
		}

		KROSS_CORE_WARN("Renderer API (None) is not supported");
		return nullptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Ref<Index> Index::Create(const char* name, uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		KROSS_CORE_WARN("Renderer API (None) is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Buffer::Index>(name, indices, size);
		}

		KROSS_CORE_WARN("Renderer API (None) is not supported");
		return nullptr;
	}

}

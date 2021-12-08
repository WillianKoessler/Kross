#include "Kross_pch.h"
#include "Buffers.h"

#include "Context.h"

namespace Kross::DirectX::Buffer {
	Vertex::Vertex(float* vertices, uint32_t size)
	{
		KROSS_PROFILE_FUNC();
		if (!vertices)
		{
			KROSS_ERROR("[Kross::DirectX::Buffer::Vertex] Creating buffer with nullptr data");
		}
	}

	Vertex::~Vertex()
	{
		KROSS_PROFILE_FUNC();
	}

	void Vertex::Bind() const
	{
		KROSS_PROFILE_FUNC();
	}

	void Vertex::unBind() const
	{
		KROSS_PROFILE_FUNC();
	}

	void Vertex::SetLayout(const Kross::Buffer::Layout & layout)
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
		if (!indices)
		{
			KROSS_ERROR("[Kross::DirectX::Buffer::Index] Creating buffer with nullptr data");
		}
	}

	Index::~Index()
	{
		KROSS_PROFILE_FUNC();
	}

	void Index::Bind() const
	{
		KROSS_PROFILE_FUNC();
	}

	void Index::unBind() const
	{
		KROSS_PROFILE_FUNC();
	}

}

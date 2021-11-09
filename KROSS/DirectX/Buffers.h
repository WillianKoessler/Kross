#pragma once

#include "Kross/Renderer/Buffer.h"
#include "Kross/Core/Core.h"
#include "DXBasics.h"

namespace Kross::DirectX::Buffer {
	class KAPI Vertex : public Kross::Buffer::Vertex
	{
	public:
		Vertex(float* vertices, uint32_t size);
		virtual ~Vertex();

		virtual void Bind() const override;
		virtual void unBind() const override;

		virtual void SetLayout(const Kross::Buffer::Layout& layout) override;
		virtual const Kross::Buffer::Layout& GetLayout() const override { return m_Layout; }

	private:
		uint32_t m_RendererID;
		Kross::Buffer::Layout m_Layout;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class KAPI Index: public Kross::Buffer::Index
	{
	public:
		Index(uint32_t* indices, uint32_t size);
		virtual ~Index();

		virtual void Bind() const override;
		virtual void unBind() const override;
		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}

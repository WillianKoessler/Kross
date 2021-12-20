#pragma once

#include "Kross/Renderer/Buffer.h"
#include "Kross/Core/Core.h"

namespace Kross::OpenGL::Buffer {
	class Vertex final : public Kross::Buffer::Vertex
	{
	public:
		Vertex(const char* name, float* vertices, uint32_t size, bool dynamic);
		virtual ~Vertex();

		virtual void Bind() const override;
		virtual void unBind() const override;

		virtual void Upload(const void* data, size_t size) const override;
		virtual void Clear() const override;

		virtual void SetLayout(const Kross::Buffer::Layout& layout) override;
		virtual const Kross::Buffer::Layout& GetLayout() const override { return m_Layout; }
		virtual const bool IsDynamic() const override { return dynamic; }
		virtual const unsigned int GetID() const override { return m_RendererID; }

	private:
		uint32_t m_RendererID = UINT32_MAX;
		uint32_t _size;
		Kross::Buffer::Layout m_Layout;
		const bool dynamic;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Index: public Kross::Buffer::Index
	{
	public:
		Index(const char* name, uint32_t* indices, uint32_t size);
		virtual ~Index();

		virtual void Bind() const override;
		virtual void unBind() const override;
		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}

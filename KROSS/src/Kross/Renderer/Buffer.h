#pragma once
#include "Kross/Core/Core.h"
#include "Kross/Core/Log.h"

namespace Kross::Buffer {

	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Bool:		return 1;
		}
		KROSS_MSGBOX("Unknown Shader Data Type", __FUNCSIG__, _ERROR_);
		return NULL;
	}
	
	struct Element
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint64_t OffSet;
		bool Normalized;

		Element()
		: Name(""), Type(ShaderDataType::None), Size(0), OffSet(0), Normalized(false)
		{}
		Element(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), OffSet(0), Normalized(normalized)
		{}

		uint32_t GetComponentCount() const
		{
			switch (Type) {
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3 * 3;
				case ShaderDataType::Mat4:    return 4 * 4;
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;
			}
			KROSS_MSGBOX("Unknown Shader Data Type", __FUNCSIG__, _ERROR_);
			return NULL;
		}
	};

	class Layout
	{
	public:
		Layout() {}
		Layout(const std::initializer_list<Element>& elements);

		inline const uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<Element> GetElements() const { return m_Elements; }

		std::vector<Element>::iterator begin()	{ return m_Elements.begin(); }
		std::vector<Element>::iterator end()	{ return m_Elements.end(); }
		std::vector<Element>::const_iterator begin() const	{ return m_Elements.begin(); }
		std::vector<Element>::const_iterator end() const	{ return m_Elements.end(); }
	private:
		void CalcOffnStride();
	private:
		std::vector<Element> m_Elements;
		uint32_t m_Stride = 0;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	class Vertex
	{
	public:
		virtual ~Vertex() {}

		virtual void Bind() const = 0;
		virtual void unBind() const = 0;

		virtual void upload(const void* data, size_t size) const = 0;

		virtual void SetLayout(const Layout& layout) = 0;
		virtual const Layout& GetLayout() const = 0;
		virtual const bool IsDynamic() const = 0;
		virtual const unsigned int GetID() const = 0;

		static Ref<Vertex> Create(float* vertices, uint32_t size, bool dynamic = false);
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Index
	{
	public:
		virtual ~Index() {};

		virtual void Bind() const = 0;
		virtual void unBind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<Index> Create(uint32_t* indices, uint32_t count);
	};
}
#include "Kross_pch.h"
#include "GLArray.h"
#include "stb_image.h"

namespace Kross::OpenGL::Texture {
	T2DArray::T2DArray(const char* name, const size_t size)
		:
		iterator(0),
		_size(size),
		glTex(new int[size])
	{
		for (int i = 0; i < size; i++) glTex[i] = 0;
		SetName(name);
	}
	T2DArray::~T2DArray()
	{
		delete[] glTex;
	}
	void T2DArray::Bind(const size_t slot)
	{
		if (!slot) {
			for (int i = 0; i < stack.size(); i++)
				glTex[i] = stack[i].get()->GetID() * stack[i].validate();
			glCall(glBindTextures((GLuint)slot, (GLsizei)stack.size(), (GLuint *)glTex));
		} else {
			KROSS_NOT_IMPLEMENTED;
		}
	}
	const int T2DArray::Get(const Ref<Kross::Texture::T2D>& texture)
	{
		if (texture)
		{
			auto i = location(texture->GetID());
			if (i != stack.end() && texture->GetID() == i->get()->GetID()) return (int)(i - stack.begin());
			KROSS_WARN("Texture '{0}' was not found in '{1}' Array.", texture->GetName(), GetName());
			Add(texture);
			return Get(texture);
		}
		KROSS_WARN("Texture is empty.");
		return -1;
	}
	void T2DArray::Add(const Ref<Kross::Texture::T2D> texture)
	{
		if (stack.size() >= _size) { KROSS_WARN("Texture Array '{0}' reached maximum capacity. Ignoring new textures.", GetName()); return; }
		if (texture)
		{
			auto i = location(texture->GetID());
			if (i != stack.end() && texture->GetID() == i->get()->GetID())
			{ KROSS_WARN("Texture '{0}' is already in the Array.", texture->GetName()); return; }

			stack.emplace(i, texture);
			KROSS_TRACE("Texture '{0}' added to Array", texture->GetName());
		}
		else
			KROSS_WARN("Adding a texture which was not yet created.");
	}
	void T2DArray::Del(const Ref<Kross::Texture::T2D> texture)
	{
		if (texture)
		{
			auto i = location(texture->GetID());
			if (i != stack.end() && i->get()->GetID() == texture->GetID())
				stack.erase(i);
			else
				KROSS_WARN("'{0}' Trying to delete texture that was not in Array.", texture->GetName());
		}
		else
			KROSS_WARN("Trying to delete with a nullptr texture.");
	}
	void T2DArray::Del(const size_t index)
	{
		if (index < stack.size())
		{
			KROSS_TRACE("'{0}' texture deleted.", (stack.begin() + index)->get()->GetName());
			stack.erase(stack.begin() + index);
		}
		else
			KROSS_WARN("Specified index is out of range.");
	}
	const size_t T2DArray::size() const
	{
		return stack.size();
	}
	const size_t T2DArray::maxSize() const
	{
		return _size;
	}
	const unsigned int* T2DArray::IDs() const
	{
		unsigned int* ptr = new GLuint[_size];
		for (int i = 0; i < _size; i++)
		{
			auto& tex = stack[i].get();
			if (tex)
			{
				ptr[i] = tex->GetID();
			}
			else
				ptr[i] = 0u;
		}

		return ptr;
	}
	void T2DArray::DebugLog() const
	{
		KROSS_WARN("**********DEBUG LOG**********");
		for (Entry e : stack)
			KROSS_TRACE("{0}", e->GetName());
		KROSS_WARN("----------DEBUG LOG----------");
	}
}

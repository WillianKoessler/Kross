#include "Kross_pch.h"
#include "_Array.h"
#include "stb_image.h"

namespace Kross::OpenGL::Texture {
	T2DArray::T2DArray(const size_t size)
		:
		iterator(0),
		_size(size),
		glTex(new int[size])
	{
		for (int i = 0; i < size; i++) glTex[i] = 0;
	}
	T2DArray::~T2DArray()
	{
		delete[] glTex;
	}
	void T2DArray::Bind(const size_t slot)
	{
		//for (int i = textures.size()-1; i > -1; i--)
		for (int i = 0; i < textures.size(); i++)
				glTex[i] = textures[i].get()->GetID() * textures[i].validate();
		glCall(glBindTextures((GLuint)slot, (GLsizei)_size, (GLuint*)glTex));
	}
	const int T2DArray::Get(const Ref<Kross::Texture::T2D>& texture)
	{
		if (texture)
		{
			auto i = location(texture->GetID());
			if (i != textures.end() && texture->GetID() == i->get()->GetID())
				return (int)(i - textures.begin());
			else if (textures.size() < _size)
			{
				textures.emplace(i, texture);
				KROSS_CORE_WARN("Texture '{0}' was not found. Added to Array.", texture->GetName());
				return (int)(location(texture->GetID()) - textures.begin());
			}
			else
				KROSS_CORE_WARN("Texture Array reached maximum capacity. Ignoring new textures.");
		}
		KROSS_CORE_WARN("Texture is empty.");
		return -1;
	}
	void T2DArray::Add(const Ref<Kross::Texture::T2D> texture)
	{
		if (textures.size() >= _size) { KROSS_CORE_WARN("Texture Array reached maximum capacity. Ignoring new textures."); return; }
		if (texture)
		{
			auto i = location(texture->GetID());
			if (i == textures.end())
			{
				textures.emplace(i, texture);
				KROSS_CORE_TRACE("Texture '{0}' added to Array", texture->GetName());
			}
			else KROSS_CORE_WARN("Texture '{0}' is already in the Array.", texture->GetName());
		}
		else
			KROSS_CORE_WARN("Adding a texture which was not yet created.");
	}
	void T2DArray::Del(const Ref<Kross::Texture::T2D> texture)
	{
		if (texture)
		{
			auto i = location(texture->GetID());
			if (i != textures.end() && i->get()->GetID() == texture->GetID())
				textures.erase(i);
			else
				KROSS_CORE_WARN("'{0}' Trying to delete texture that was not in Array.", texture->GetName());
		}
		else
			KROSS_CORE_WARN("Trying to delete with a nullptr texture.");
	}
	void T2DArray::Del(const size_t index)
	{
		if (index < textures.size())
		{
			KROSS_CORE_TRACE("'{0}' texture deleted.", (textures.begin() + index)->get()->GetName());
			textures.erase(textures.begin() + index);
		}
		else
			KROSS_CORE_WARN("Specified index is out of range.");
	}
	const size_t T2DArray::size() const
	{
		return textures.size();
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
			auto& tex = textures[i].get();
			if (tex)
			{
				ptr[i] = tex->GetID();
			}
			else
				ptr[i] = 0u;
		}

		return ptr;
	}
}
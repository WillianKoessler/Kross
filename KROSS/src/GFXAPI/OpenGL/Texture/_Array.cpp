#include "Kross_pch.h"
#include "_Array.h"
#include "stb_image.h"
#include "GFXAPI/OpenGL/Context.h"

namespace Kross::OpenGL::Texture {
	T2DArray::T2DArray(const size_t size)
		:
		iterator(0),
		_size(size),
		glTex(new unsigned int[size])
	{
		textures.resize(size);
	}
	T2DArray::~T2DArray()
	{
		delete[] glTex;
	}
	void T2DArray::Bind(const size_t slot) const
	{
		for (int i = 0; i < _size; i++)
			if (textures[i])
				glTex[i] = textures[i]->GetID();
			else
				glTex[i] = 0;
		glCall(glBindTextures((GLuint)slot, (GLsizei)_size, (const GLuint*)glTex));
	}
	const unsigned int T2DArray::Get(const Ref<Kross::Texture::T2D> tex)
	{
		if (tex)
		{
			auto i = textures.begin();
			for (; i != textures.end() && *i; i++)
				if ((*i)->GetID() == tex->GetID())
					return (unsigned int)std::distance(textures.begin(), i);
			textures.insert(i, tex);
		}
		return 0;
	}
	void T2DArray::Add(const Ref<Kross::Texture::Base> texture)
	{
		if (texture && texture->GetID())
		{
			auto tex = std::dynamic_pointer_cast<Kross::Texture::T2D>(texture);
			if (tex) textures[iterator++] = tex;
			else { KROSS_CORE_ERROR("{0} Texture is not T2D.", __FUNCSIG__); }
		}
		else
			KROSS_CORE_WARN("{0} Adding a texture which was not yet created.", __FUNCSIG__);
	}
	void T2DArray::Del(const Ref<Kross::Texture::Base> texture)
	{
		if (texture)
		{
			if (texture == Ref<Kross::Texture::T2D>())
			{
				for (auto i = textures.begin(); i != textures.end(); i++)
					if ((*i)->GetID() == texture->GetID())
						textures.erase(i);
			}
			else
				KROSS_CORE_ERROR_("[Kross::OpenGL::Texture::T2DArray] Texture is not a T2D texture.");
		}
		else
			KROSS_CORE_WARN("[Kross::OpenGL::Texture::T2DArray] Trying to delete with a nullptr texture.");
	}
	void T2DArray::Del(const size_t index)
	{
		if (index < _size)
		{
			size_t i = 0;
			for (auto iter = textures.begin(); iter != textures.end(); iter++)
			{
				if (i == index)
					if ((*iter)->GetID())
						textures.erase(iter);
					else
						KROSS_CORE_WARN("[Kross::OpenGL::Texture::T2DArray] Trying to delete a non existing texture.");
				else i++;
			}
		}
		else
			KROSS_CORE_ERROR_("[Kross::OpenGL::Texture::T2DArray] Specified index is out of range.");
	}
	const size_t T2DArray::size() const
	{
		return _size;
	}
	const size_t* T2DArray::IDs() const
	{
		GLuint* ptr = new GLuint[_size];
		for (int i = 0; i < _size; i++)
		{
			auto& tex = textures[i];
			if (tex)
			{
				ptr[i] = tex->GetID();
			}
			else
				ptr[i] = 0u;
		}

		return (size_t*)ptr;
	}
}
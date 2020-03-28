#include "Kross_pch.h"
#include "_Array.h"
#include "stb_image.h"
#include "GFXAPI/OpenGL/GLErrors.h"
#include "GFXAPI/OpenGL/Context.h"

namespace Kross::OpenGL::Texture {
	T2DArray::T2DArray(const size_t size)
		:
		currentHead(0),
		_size(size),
		textures(new Ref<Kross::Texture::T2D>[size]),
		glTex(new unsigned int[size])
	{}
	T2DArray::~T2DArray()
	{
		delete[] glTex;
	}
	void T2DArray::Bind(const size_t slot) const
	{
		GLerror();
		for (int i = 0; i < _size; i++)
			if (textures[i])
				glTex[i] = textures[i]->GetID();
			else
				glTex[i] = 0;
		glBindTextures((GLuint)slot, (GLsizei)_size, (const GLuint*)glTex);
	}
	const unsigned int T2DArray::Get(const Ref<Kross::Texture::T2D> tex)
	{
		if (tex)
		{
			bool found = false;
			unsigned int i = 0;
			for (; textures[i] && i < _size; i++)
				if (textures[i]->GetID() == tex->GetID())
				{
					found = true;
					break;
				}
			if (!found) textures[i] = tex;
			return i;
		}
		return 0;
	}
	void T2DArray::Add(const Ref<Kross::Texture::Base> texture)
	{
		if (texture && texture->GetID())
		{
			auto tex = std::dynamic_pointer_cast<Kross::Texture::T2D>(texture);
			if (tex)
			{
				if (currentHead + 1 < _size)
					textures[currentHead++] = tex;
				else
					KROSS_CORE_WARN("[ {0} ] |||| Texture Array Overflow. Newer textures ignored!!!", __FUNCTION__);
			}
			else { KROSS_CORE_ERROR("[ {0} ] |||| Texture is not T2D.", __FUNCTION__); }
		}
		else
			KROSS_CORE_WARN("[ {0} ] |||| Adding a texture which was not yet created.", __FUNCTION__);
	}
	void T2DArray::Del(const Ref<Kross::Texture::Base> texture)
	{
		if (texture)
		{
			if (texture == Ref<Kross::Texture::T2D>())
			{
				for (auto i = 0u; i < _size; i++)
					if (textures[i]->GetID() == texture->GetID())
						textures[i] = nullptr;
			}
			else
				KROSS_CORE_ERROR_("[ {0} ] |||| Texture is not a T2D texture.", __FUNCTION__);
		}
		else
			KROSS_CORE_WARN("[ {0} ] |||| Trying to delete with a nullptr texture.", __FUNCTION__);
	}
	void T2DArray::Del(const size_t index)
	{
		if (index < _size)
			if (textures[index]->GetID())
				textures[index] = nullptr;
			else
				KROSS_CORE_WARN("[ {0} ] |||| Trying to delete a non existing texture.", __FUNCTION__);
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
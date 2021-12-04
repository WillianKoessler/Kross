#include <Kross_pch.h>
#include "Stack.h"

#include "Kross/Core/Stack_Impl.h"

#include "Kross/Renderer/Shaders.h"
#include "Kross/Renderer/Textures/Textures.h"
//#include "Kross/Core/Layer.h"
#include "Kross/Renderer/Mesh/Mesh.h"

#define __impl__STACK_TEMPLATE(x)\
std::vector<Stack_Impl<##x>::Entry> Stack_Impl<##x>::stack;\
std::string Stack_Impl<##x>::Entry::table = "";\
template class Stack<##x>;

namespace Kross {
	__impl__STACK_TEMPLATE(Shader);
	__impl__STACK_TEMPLATE(Texture::T2D);
	//__impl__STACK_TEMPLATE(Layer);
	//__impl__STACK_TEMPLATE(Mesh::Base);

	template<typename T>
	Stack<T>& Kross::Stack<T>::instance()
	{
		return Stack_Impl<T>::instance();
	}
}
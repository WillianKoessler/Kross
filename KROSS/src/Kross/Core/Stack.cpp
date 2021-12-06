#include <Kross_pch.h>
#include "Stack.h"

#include "Kross/Core/Stack.h"

#include "Kross/Renderer/Shaders.h"
#include "Kross/Renderer/Textures/Textures.h"
//#include "Kross/Core/Layer.h"
#include "Kross/Renderer/Mesh/Mesh.h"

#define __impl__STACK_TEMPLATE(x)\
std::vector<Stack<##x>::Entry> Stack<##x>::stack;\
std::string Stack<##x>::Entry::table = "";\
template class Stack<##x>;

namespace Kross {
	__impl__STACK_TEMPLATE(Shader);
	__impl__STACK_TEMPLATE(Texture::T2D);
	//__impl__STACK_TEMPLATE(Layer);
	//__impl__STACK_TEMPLATE(Mesh::Base);
}
#include "Kross_pch.h"
#include "Stack_Impl.h"

#define __impl__STACK_TEMPLATE(x)\
std::vector<Stack_Impl<##x>::Entry> Stack_Impl<##x>::stack;\
std::string Stack_Impl<##x>::Entry::table = "";\
template class Stack<##x>;

namespace Kross {
}

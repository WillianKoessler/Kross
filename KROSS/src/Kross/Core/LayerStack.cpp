#include <Kross_pch.h>
#include "LayerStack.h"
#include "Kross/Core/LayerStack_Impl.h"

namespace Kross {
	static std::vector<LayerStack_Impl*> ptrs;
	LayerStack* LayerStack::Create()
	{
		ptrs.push_back(new LayerStack_Impl());
		return ptrs.back();
	}
	void LayerStack::Delete()
	{
		ptrs.pop_back();
	}
}
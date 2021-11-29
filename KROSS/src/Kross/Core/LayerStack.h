#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Layer.h"

namespace Kross {
	class KAPI LayerStack
	{
	public:
		virtual ~LayerStack() {};
		virtual void PushLayer(const Ref<Layer>& layer) = 0;
		virtual void PushOverlay(const Ref<Layer>& overlay) = 0;
		virtual void PopLayer(const Ref<Layer>& layer) = 0;
		virtual void PopOverlay(const Ref<Layer>& overlay) = 0;
		virtual void clear() = 0;
		virtual uint64_t size() const = 0;
		virtual Ref<Layer> at(size_t index) = 0;
		static LayerStack* Create();
		static void Delete();
	};
}

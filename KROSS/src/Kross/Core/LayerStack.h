#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Layer.h"
#include "Kross/Core/LayerStack.h"

namespace Kross {
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PropagateEvent(class Event &e);
		void PushLayer	(const Ref<Layer>& layer);
		void PushOverlay(const Ref<Layer>& overlay);
		void PopLayer	(const Ref<Layer>& layer);
		void PopOverlay(const Ref<Layer>& overlay);
		void clear();

		std::vector<Ref<Layer>>::iterator begin() { return m_vecLayers.begin(); }
		std::vector<Ref<Layer>>::iterator end() { return m_vecLayers.end(); }
		std::vector<Ref<Layer>>::reverse_iterator rbegin() { return m_vecLayers.rbegin(); }
		std::vector<Ref<Layer>>::reverse_iterator rend() { return m_vecLayers.rend(); }

	private:
		std::vector<Ref<Layer>> m_vecLayers;
		uint32_t m_unLayersInsertIndex = 0;
	};
}
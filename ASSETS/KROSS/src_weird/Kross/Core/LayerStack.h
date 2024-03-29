#pragma once

#include "Core.h"
#include "Layer.h"

#include <vector>

namespace Kross {
	class KAPI LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer	(const Ref<Layer>& layer);
		void PushOverlay(const Ref<Layer>& overlay);
		void PopLayer	(const Ref<Layer>& layer);
		void PopOverlay	(const Ref<Layer>& overlay);

		std::vector<Ref<Layer>>::iterator begin() { return m_vecLayers.begin(); }
		std::vector<Ref<Layer>>::iterator end() { return m_vecLayers.end(); }
	private:
		std::vector<Ref<Layer>> m_vecLayers;
		unsigned int m_unLayersInsertIndex = 0;
	};
}
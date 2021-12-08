#include <Kross_pch.h>
#include "LayerStack.h"

namespace Kross {
	LayerStack::LayerStack()
	{
		KROSS_INFO("LayerStack Constructed");
	}

	LayerStack::~LayerStack()
	{
		KROSS_PROFILE_FUNC();
		for (auto& layer : m_vecLayers)
		{
			std::string name = layer->GetName();
			KROSS_INFO("Deleting layer '{0}'", name);
			layer.reset();
			KROSS_INFO("Layer '{0}' deleted", name);
		}
	}

	void LayerStack::PushLayer(const Ref<Layer>& layer)
	{
		m_vecLayers.emplace(m_vecLayers.begin() + m_unLayersInsertIndex, layer);
		m_unLayersInsertIndex++;
	}

	void LayerStack::PushOverlay(const Ref<Layer>& overlay)
	{
		m_vecLayers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(const Ref<Layer>& layer)
	{
		auto it = std::find(m_vecLayers.begin(), m_vecLayers.end() + m_unLayersInsertIndex, layer);
		if (it != m_vecLayers.end() + m_unLayersInsertIndex)
		{
			m_vecLayers.erase(it);
			m_unLayersInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(const Ref<Layer>& overlay)
	{
		auto it = std::find(m_vecLayers.begin() + m_unLayersInsertIndex, m_vecLayers.end(), overlay);
		if (it != m_vecLayers.end())
		{
			m_vecLayers.erase(it);
		}
	}
	void LayerStack::clear()
	{
		m_vecLayers.clear();
	}
}
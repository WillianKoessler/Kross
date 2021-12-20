#include <Kross_pch.h>
#include "LayerStack.h"
#include "Kross/Events/Event.h"

namespace Kross {
	LayerStack::LayerStack()
	{
		KROSS_INFO("LayerStack Constructed");
	}

	LayerStack::~LayerStack()
	{
		KROSS_PROFILE_FUNC();
		for (auto &layer : m_vecLayers)
			KROSS_INFO("Layer '{0}' deleted", layer->GetName());
	}

	void LayerStack::PropagateEvent(Event &e)
	{
		for (auto layer = rbegin(); !((layer == rend()) || (e.handled)); ++layer)
			if ((*layer)->IsPassingEvents())
				(*layer)->OnEvent(e);
	}

	void LayerStack::PushLayer(const Ref<Layer> &layer)
	{
		m_vecLayers.emplace(m_vecLayers.begin() + m_unLayersInsertIndex, layer);
		m_unLayersInsertIndex++;
	}

	void LayerStack::PushOverlay(const Ref<Layer> &overlay)
	{
		m_vecLayers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(const Ref<Layer> &layer)
	{
		auto it = std::find(m_vecLayers.begin(), m_vecLayers.end() + m_unLayersInsertIndex, layer);
		if (it != m_vecLayers.end() + m_unLayersInsertIndex)
		{
			m_vecLayers.erase(it);
			m_unLayersInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(const Ref<Layer> &overlay)
	{
		auto it = std::find(m_vecLayers.begin() + m_unLayersInsertIndex, m_vecLayers.end(), overlay);
		if (it != m_vecLayers.end())
		{
			m_vecLayers.erase(it);
		}
	}
	void LayerStack::clear()
	{
		for (auto &l : m_vecLayers) l->OnDetach();
		m_vecLayers.clear();
	}
}

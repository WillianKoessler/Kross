#include <Kross_pch.h>
#include "LayerStack_Impl.h"

namespace Kross {
	LayerStack_Impl::LayerStack_Impl()
	{
		KROSS_CORE_INFO("LayerStack created");
	}

	LayerStack_Impl::~LayerStack_Impl()
	{
		KROSS_PROFILE_FUNC();
		for (auto& layer : m_vecLayers)
		{
			std::string name = layer->GetName();
			KROSS_CORE_INFO("Deleting layer '{0}'", name);
			layer.reset();
			KROSS_CORE_INFO("Layer '{0}' deleted", name);
		}
	}

	void LayerStack_Impl::PushLayer(const Ref<Layer>& layer)
	{
		m_vecLayers.emplace(m_vecLayers.begin() + m_unLayersInsertIndex, layer);
		m_unLayersInsertIndex++;
	}

	void LayerStack_Impl::PushOverlay(const Ref<Layer>& overlay)
	{
		m_vecLayers.emplace_back(overlay);
	}

	void LayerStack_Impl::PopLayer(const Ref<Layer>& layer)
	{
		auto it = std::find(m_vecLayers.begin(), m_vecLayers.end() + m_unLayersInsertIndex, layer);
		if (it != m_vecLayers.end() + m_unLayersInsertIndex)
		{
			m_vecLayers.erase(it);
			m_unLayersInsertIndex--;
		}
	}

	void LayerStack_Impl::PopOverlay(const Ref<Layer>& overlay)
	{
		auto it = std::find(m_vecLayers.begin() + m_unLayersInsertIndex, m_vecLayers.end(), overlay);
		if (it != m_vecLayers.end())
		{
			m_vecLayers.erase(it);
		}
	}
	void LayerStack_Impl::clear()
	{
		m_vecLayers.clear();
	}
	uint64_t LayerStack_Impl::size() const
	{
		return m_vecLayers.size();
	}
	Ref<Layer> LayerStack_Impl::at(size_t index)
	{
		return m_vecLayers[index];
	}
}
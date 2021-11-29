#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Layer.h"
#include "Kross/Core/LayerStack.h"

namespace Kross {
	class LayerStack_Impl : public LayerStack
	{
	public:
		LayerStack_Impl();
		virtual ~LayerStack_Impl() override;

		virtual void PushLayer	(const Ref<Layer>& layer) override;
		virtual void PushOverlay(const Ref<Layer>& overlay) override;
		virtual void PopLayer	(const Ref<Layer>& layer) override;
		virtual void PopOverlay(const Ref<Layer>& overlay) override;
		virtual void clear() override;
		virtual uint64_t size() const override;
		virtual Ref<Layer> at(size_t index) override;
	private:
		std::vector<Ref<Layer>> m_vecLayers;
		uint32_t m_unLayersInsertIndex = 0;
	};
}
#pragma once
#include "Kross.h"

#include "Panels/Panel.h"

namespace Kross {
	class UI final : public Layer
	{
		Ref<Texture::T2D> canvas;
		std::vector<Panel*> m_pPanels;
	public:
		UI();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(double) override;
		virtual void OnImGuiRender(double) override;
		virtual void OnEvent(Event& event) override;
		void setCanvas(const Ref<Texture::T2D>& texture);
	};
}

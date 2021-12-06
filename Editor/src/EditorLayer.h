#pragma once
#include "Kross.h"

#include "Panels/Panel.h"

namespace Kross {
	class EditorLayer final : public Layer
	{
		Ref<FrameBuffer> m_Frame;
		Ref<Camera::Controller> m_Camera;
		std::vector<Panel*> m_pPanels;
	public:
		EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(double) override;
		virtual void OnImGuiRender(double) override;
		virtual void OnEvent(Event& event) override;
	};
}

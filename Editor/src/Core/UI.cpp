#include "UI.h"

#include "Panels/DockSpace.h"
#include "Panels/RendererStats.h"
#include "Panels/Viewport.h"
#include <glm/gtc/type_ptr.hpp>

namespace Kross {
	UI::UI() : Layer("UI"), canvas(nullptr)
	{
	}

	void UI::OnAttach()
	{
		RenderCommand::SetClear(0x0f0f0fff);
		m_pPanels.emplace_back(new DockSpace("Kross"));
		m_pPanels.emplace_back(new RendererStats("Renderer2D Status"));
		m_pPanels.emplace_back(new Viewport("Viewport", canvas));
	}
	void UI::OnDetach()
	{
		for (int i = (int)m_pPanels.size() - 1; i > -1; i--) delete m_pPanels[i];
	}

	void UI::OnUpdate(double ts)
	{
		Renderer2D::ResetStats();
		RenderCommand::Clear();
	}

	void UI::OnImGuiRender(double ts)
	{
		for(auto& p : m_pPanels) p->Show(ts);
	}

	void UI::OnEvent(Event& event)
	{
	}

	void UI::setCanvas(const Ref<Texture::T2D>& texture)
	{
		canvas.reset(texture.get());
	}
}

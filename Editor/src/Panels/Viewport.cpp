#include "Viewport.h"

namespace Kross {
	Viewport::Viewport(const char* name, const Ref<FrameBuffer>& frame)
		: m_Canvas(frame), m_ViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight())
	{
		SetName(name);
		KROSS_INFO("Viewport Panel '{0}' Constructed", name);
	}

	void Viewport::Show(double ts)
	{
		if (!Manager().s_bViewport) return;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin(GetName(), &Manager().s_bViewport, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize.x != viewportPanelSize.x && m_ViewportSize.y != viewportPanelSize.y)
		{
			if(m_Canvas) m_Canvas->Resize(m_ViewportSize);
			m_ViewportSize = { (uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y };
		}
		void *image;
		if (m_Canvas) {
			image = (void *)(uintptr_t)m_Canvas->GetColorAttachmentID();
		} else {
			image = (void *)(uintptr_t)Stack<Texture::T2D>::Get("mamma")->GetID();
		}
		ImGui::Image(image, ImVec2((float)m_ViewportSize.x, (float)m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PopStyleVar(3);
		ImGui::End();
	}
}
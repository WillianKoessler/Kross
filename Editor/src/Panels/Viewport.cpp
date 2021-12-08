#include "Viewport.h"

namespace Kross {
	Viewport::Viewport(const char *name, const Ref<FrameBuffer> &frame, Camera::Editor &camera)
		: m_Canvas(frame), m_ViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight()), p_editorCamera(camera)
	{
		m_Placeholder = (void *)(uintptr_t)Stack<Texture::T2D>::Get("mamma")->GetID();
		m_Flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;
		SetName(name);
		KROSS_INFO("Viewport Panel '{0}' Constructed", name);
	}

	void Viewport::Menu()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Settings")) {
				ImGui::MenuItem("BackFaceCulling", NULL, &m_bBackFaceCull);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void Viewport::Show(double ts)
	{
		if (!Manager().s_bViewport) return;
		RenderCommand::BackCull(m_bBackFaceCull);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin(GetName(), &Manager().s_bViewport, m_Flags);
		Menu();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2 *)&viewportPanelSize && viewportPanelSize.x > 0.0f && viewportPanelSize.y > 0.0f)
		{
			m_ViewportSize = { (uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y };
			p_editorCamera.SetViewportSize(m_ViewportSize);
			if (m_Canvas) m_Canvas->Resize(m_ViewportSize);
		}

		void *image;
		if (m_Canvas) image = (void *)(uintptr_t)m_Canvas->GetColorAttachmentID();
		else image = m_Placeholder;
		ImGui::Image(image, viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PopStyleVar(3);
		ImGui::End();
	}
}
#include "Editor_pch.h"
#include "EditorLayer.h"
#include "Panels/Panel.h"

namespace Kross {
	void EditorLayer::Viewport()
	{
		if (Panel::Manager().s_bViewport) {
			static bool m_bBackFaceCull = false;
			static bool m_bWireFrame = false;
			static auto m_Flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration;

			RenderCommand::BackCull(m_bBackFaceCull);
			RenderCommand::SetMode(m_bWireFrame ? RendererAPI::Mode::Wireframe : RendererAPI::Mode::Fill);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::Begin("Viewport", &Panel::Manager().s_bViewport, m_Flags);

			Application::Get().GetGUILayer()->BlockEvents(!ImGui::IsWindowFocused() && !ImGui::IsWindowHovered());

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Settings")) {
					ImGui::MenuItem("BackFaceCulling", NULL, &m_bBackFaceCull);
					ImGui::MenuItem("WireFrame", NULL, &m_bWireFrame);
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImVec2 ImGuiViewportPanelSize = ImGui::GetContentRegionAvail();
			glm::vec2 glmViewportSize = { ImGuiViewportPanelSize.x, ImGuiViewportPanelSize.y };

			if (m_ViewportSize != glmViewportSize && ImGuiViewportPanelSize.x > 0.0f && ImGuiViewportPanelSize.y > 0.0f) {
				m_ViewportSize = glmViewportSize;
				m_Scene.OnViewportResize(m_ViewportSize);
			}

			ImGui::Image(
				reinterpret_cast<void*>((uintptr_t)m_Frame->GetColorAttachmentID(0)),
				ImGuiViewportPanelSize,
				ImVec2(0.0f, 1.0f),
				ImVec2(1.0f, 0.0f)
			);

			ShowGuizmo();

			ImGui::PopStyleVar(3);
			ImGui::End();
		}
	}
}

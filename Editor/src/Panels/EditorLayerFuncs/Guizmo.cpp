#include "Editor_pch.h"
#include "EditorLayer.h"
#include "Kross/Math/Math.h"
#include <glm/glm/gtc/type_ptr.hpp>

namespace Kross {
	void Kross::EditorLayer::ShowGuizmo()
	{
		if (m_Scene.Selected()) {
			if (m_Scene.Selected().Has<TransformComponent>() != 1) return;

			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			static glm::vec2 m_ViewportBounds[2];
			m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
			m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			const glm::mat4 &cameraProj = m_Camera.GetProjMat();
			glm::mat4 &cameraView = m_Camera.GetViewMat();

			auto cmp = m_Scene.Selected().Get<TransformComponent>();
			glm::mat4 transform = (glm::mat4)(*cmp);

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProj),
				(ImGuizmo::OPERATION)m_GuizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform));

			if (ImGuizmo::IsUsing()) {
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);
				glm::vec3 deltaRotation = rotation - cmp->Rotation;
				cmp->Position = translation;
				cmp->Rotation += deltaRotation;
				cmp->Scale = scale;
			}
		}
	}
}

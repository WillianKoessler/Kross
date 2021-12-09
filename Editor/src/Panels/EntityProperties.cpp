#include "Editor_pch.h"
#include "Kross/Util/Util.h"
#include "EntityProperties.h"
#include <glm/glm/gtc/type_ptr.hpp>

namespace Kross {
	EntityProperties::EntityProperties(const Ref<Scene> &scene)
		: p_Scene(scene)
	{
		m_strName = "Properties Panel";
		KROSS_INFO("Panel '{0}' Constructed", m_strName);
	}

	void EntityProperties::Show(double ts)
	{
		if (!Manager().s_bPropertiesInspector) return;
		ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
		if (ImGui::Begin(m_strName, &Panel::Manager().s_bPropertiesInspector))
			DrawEntity(s_Selection);
		ImGui::End();
	}
	void EntityProperties::DrawEntity(Entity &entity)
	{
		if (entity) {
			DrawSelectionComponent<TagComponent>("Tag: ", [](const char *id, TagComponent *cmp) {
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), cmp->tag);
				if (ImGui::InputText(id, buffer, sizeof(buffer)) && Input::IsKeyPressed(Key::Enter))
					strcpy_s(cmp->tag, TagComponent::limit, buffer);
				});
			DrawSelectionComponent<TransformComponent>("", [](const char *id, TransformComponent *cmp) {
				if (ImGui::TreeNodeEx((const void *)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Tranform")) {
					ImGui::Text("Position: "); ImGui::SameLine();
					ImGui::DragFloat3("##Position: ", glm::value_ptr(cmp->Transform[3]), 0.1f);
					ImGui::TreePop();
				}
				});
			DrawSelectionComponent<SpriteComponent>("Sprite: ", [](const char *id, SpriteComponent *cmp) {
				ImGui::ColorEdit4("##Sprite: ", glm::value_ptr(cmp->tint));
				});
			DrawSelectionComponent<CameraComponent>("Camera: ", [](const char *cid, CameraComponent *cmp) {
				SceneCamera &camera = cmp->camera;
				std::string id(cid);

				ImGui::Text(" ");
				const char *projTypeStr[] = { "Perspective", "Orthographic" };
				const char *selection = projTypeStr[(int)cmp->camera.GetProjType()];
				ImGui::Text("Projection Type: "); ImGui::SameLine(); if (ImGui::BeginCombo("##Projection Type: ", selection, ImGuiComboFlags_NoArrowButton)) {
					for (int i = 0; i < sizeof(projTypeStr) / sizeof(const char *); i++) {
						bool isSelected = selection == projTypeStr[i];
						if (ImGui::Selectable(projTypeStr[i], isSelected)) {
							selection = projTypeStr[i];
							camera.SetProjType((SceneCamera::ProjectionType)i);
						}
						if (isSelected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				ImGui::Text("Active Camera: "); ImGui::SameLine(); ImGui::Checkbox("##Active Camera: ", &cmp->Active);

				if (cmp->camera.GetProjType() == SceneCamera::ProjectionType::Orthographic) {
					float fSize = camera.OrthoSize();
					float fNear = camera.GetNearClip();
					float fFar = camera.GetFarClip();
					ImGui::Text("OrthoSize: "); ImGui::SameLine(); if (ImGui::DragFloat("##OrthographicOrthoSize: ", &fSize, 0.1f)) camera.SetOrthoSize(fSize);
					ImGui::Text("NearClip: ");  ImGui::SameLine(); if (ImGui::DragFloat("##OrthographicNearClip: ", &fNear, 0.1f)) camera.SetNearClip(fNear);
					ImGui::Text("FarClip: ");   ImGui::SameLine(); if (ImGui::DragFloat("##OrthographicFarClip: ", &fFar, 0.1f)) camera.SetFarClip(fFar);
				}
				if (cmp->camera.GetProjType() == SceneCamera::ProjectionType::Perspective) {
					float fFOV = glm::degrees(camera.GetPerspVerticalFOV());
					float fNear = camera.GetNearClip();
					float fFar = camera.GetFarClip();
					ImGui::Text("FOV: ");		ImGui::SameLine(); if (ImGui::DragFloat("##PerspectiveFOV: ", &fFOV, 0.1f)) camera.SetPerspVerticalFOV(glm::radians(fFOV));
					ImGui::Text("NearClip: ");  ImGui::SameLine(); if (ImGui::DragFloat("##PerspectiveNearClip: ", &fNear, 0.1f)) camera.SetNearClip(fNear);
					ImGui::Text("FarClip: ");   ImGui::SameLine(); if (ImGui::DragFloat("##PerspectiveFarClip: ", &fFar, 0.1f)) camera.SetFarClip(fFar);
				}
				});
		}
	}
}
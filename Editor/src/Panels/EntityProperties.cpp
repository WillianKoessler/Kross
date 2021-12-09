#include "Editor_pch.h"
#include "Kross/Util/Util.h"
#include "EntityProperties.h"
#include <glm/glm/gtc/type_ptr.hpp>

namespace Kross {
	const float EntityProperties::s_DisabledColor = phi<float>();

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
			DrawSelectionComponent<TagComponent>("Tag", [](const Ref<Scene> &scene, const char *id, TagComponent *cmp) {
				char buffer[TagComponent::limit];
				memset(buffer, 0, TagComponent::limit);
				strcpy_s(buffer, TagComponent::limit, cmp->tag);
				if (ImGui::InputText(id, buffer, TagComponent::limit) && Input::IsKeyPressed(Key::Enter))
					strcpy_s(cmp->tag, TagComponent::limit, buffer);
				});
			DrawSelectionComponent<TransformComponent>("Transform", [](const Ref<Scene> &scene, const char *id, TransformComponent *cmp) {
				ImGui::Text("Position: "); ImGui::SameLine(); ImGui::DragFloat3("##Position: ", glm::value_ptr(cmp->Position), 0.1f);
				ImGui::Text("Rotation: "); ImGui::SameLine(); ImGui::DragFloat3("##Rotation: ", glm::value_ptr(cmp->Rotation), 0.1f);
				ImGui::Text("Scale: "); ImGui::SameLine(); ImGui::DragFloat3("##Scale: ", glm::value_ptr(cmp->Scale), 0.1f);
				});
			DrawSelectionComponent<SpriteComponent>("Sprite", [](const Ref<Scene> &scene, const char *id, SpriteComponent *cmp) {
				ImGui::ColorEdit4("##Sprite: ", glm::value_ptr(cmp->tint));
				});
			DrawSelectionComponent<CameraComponent>("Camera", [](const Ref<Scene> &scene, const char *id, CameraComponent *cmp) {
				SceneCamera &camera = cmp->camera;
				if (s_Selection) {
					Entity activeCamera = scene->GetCurrentCamera();
					ImGui::Text("Current Active Camera: %s", activeCamera.Has<TagComponent>() == 1 ? activeCamera.Get<TagComponent>()->tag : "(null)");
					ImGui::Text("Active Camera: "); ImGui::SameLine();
					if (s_Selection.Has<TagComponent>() == 1 && s_Selection.Has<TransformComponent>() == 1) {
						if (ImGui::Button("SET")) scene->SetPrimaryCamera(s_Selection);
					} else {
						ImGui::SameLine();
						EntityProperties::SetDisabledStyle(ImGuiCol_Button, ImGuiCol_ButtonHovered, ImGuiCol_ButtonActive);
						ImGui::Button("SET");
						ImGui::PopStyleColor(3);
					}
				}

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


				if (cmp->camera.GetProjType() == SceneCamera::ProjectionType::Orthographic) {
					ImGui::Text("Fixed Aspect Ratio"); ImGui::SameLine(); ImGui::Checkbox("##Fixed Aspect Ratio", &cmp->fixedAspectRatio);

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
	void EntityProperties::SetDisabledStyle(int item, int hovered, int activated)
	{
		glm::vec4 color = *(glm::vec4 *)&ImGui::GetStyleColorVec4(item);
		color /= s_DisabledColor;
		if (item > -1 && item < ImGuiCol_COUNT) ImGui::PushStyleColor(item, *(ImVec4 *)&color);
		if (hovered > -1 && hovered < ImGuiCol_COUNT) ImGui::PushStyleColor(hovered, *(ImVec4 *)&color);
		if (activated > -1 && activated < ImGuiCol_COUNT) ImGui::PushStyleColor(activated, *(ImVec4 *)&color);
	}
	//void EntityProperties::SetEnabledStyle(int item, int hovered, int activated)
	//{
	//	glm::vec4 color = *(glm::vec4 *)&ImGui::GetStyleColorVec4(item);
	//	color *= s_DisabledColor;
	//	ImGui::PushStyleColor(item, *(ImVec4 *)&color);
	//	if (hovered > -1 && hovered < ImGuiCol_COUNT) ImGui::PushStyleColor(hovered, *(ImVec4 *)&color);
	//}
}
#include "Editor_pch.h"
#include "Kross/Util/Util.h"
#include "EntityProperties.h"
#include <glm/glm/gtc/type_ptr.hpp>

#include <imgui/imgui_internal.h>

static constexpr float m_fGlobalLabelsWidth = 100.0f;
namespace Kross {
	/*
	static void DrawColorControl(const std::string &label, glm::vec4 &values, float resetValue = 0.0f, float speed = 0.1f, float columnWidth = 100.0f)
	{
		using namespace ImGui;

		Columns(2, label.c_str());
		SetColumnWidth(0, columnWidth);
		PushID(label.c_str());
		Text(label.c_str());

		NextColumn();
		PushMultiItemsWidths(3, CalcItemWidth());

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight * phi<float>(), lineHeight };
		static const ImVec2 inBetween = { phi<float>() * phi<float>() , 0.0f };
		static const ImVec2 buttonToDrag = { phi<float>(), 0.0f };
		static const glm::vec4 Rcolor = { 0.838096f, 0.161903f, 0.161903f, 1.0f };
		static const glm::vec4 Gcolor = { 0.161903f, 0.838096f, 0.161903f, 1.0f };
		static const glm::vec4 Bcolor = { 0.161903f, 0.161903f, 0.838096f, 1.0f };
		static const glm::vec4 Acolor = { 0.838096f, 0.838096f, 0.838096f, 1.0f };
		auto part = [&](const std::string &str, glm::vec4 color) {
			PushStyleColor(ImGuiCol_Button, *(ImVec4 *)&color);
			PushStyleColor(ImGuiCol_ButtonHovered, *(ImVec4 *)&(color * phi<float>()));
			PushStyleColor(ImGuiCol_ButtonActive, *(ImVec4 *)&(color * phi<float>() * phi<float>()));
			PushStyleVar(ImGuiStyleVar_ItemSpacing, buttonToDrag);
			if (Button((str+ "##" + label).c_str()))
				values.r = resetValue;
			PopStyleColor(3);
			PopStyleVar();
			ImGui::SameLine();

			PushStyleVar(ImGuiStyleVar_ItemSpacing, inBetween);
			ImGui::DragFloat(("##" + str + label).c_str(), &values.r, speed);
			PopStyleVar();
			PopItemWidth();
			ImGui::SameLine();
		};
		
		part("R", Rcolor);
		part("G", Gcolor);
		part("B", Bcolor);
		part("A", Acolor);

		static const glm::vec4 Xcolor = { 0.838096f, 0.161903f, 0.161903f, 1.0f };
		PushStyleColor(ImGuiCol_Button, *(ImVec4 *)&Xcolor);
		PushStyleColor(ImGuiCol_ButtonHovered, *(ImVec4 *)&(Xcolor * phi<float>()));
		PushStyleColor(ImGuiCol_ButtonActive, *(ImVec4 *)&(Xcolor * phi<float>() * phi<float>()));
		PushStyleVar(ImGuiStyleVar_ItemSpacing, buttonToDrag);
		if (Button(("X##" + label).c_str()))
			values.x = resetValue;
		PopStyleColor(3);
		ImGui::SameLine();
		PopStyleVar();

		PushStyleVar(ImGuiStyleVar_ItemSpacing, inBetween);
		ImGui::DragFloat(("##X" + label).c_str(), &values.x, speed);
		PopItemWidth();
		ImGui::SameLine();
		PopStyleVar();


		//static const glm::vec4 Rcolor = { 0.838096f, 0.161903f, 0.161903f, 1.0f };
		//PushStyleColor(ImGuiCol_Button, *(ImVec4 *)&Rcolor);
		//PushStyleColor(ImGuiCol_ButtonHovered, *(ImVec4 *)&(Rcolor * phi<float>()));
		//PushStyleColor(ImGuiCol_ButtonActive, *(ImVec4 *)&(Rcolor * phi<float>() * phi<float>()));
		//PushStyleVar(ImGuiStyleVar_ItemSpacing, buttonToDrag);
		//if (Button(("R##" + label).c_str())) values.r = resetValue; PopStyleColor(3); ImGui::SameLine(); PopStyleVar(); PushStyleVar(ImGuiStyleVar_ItemSpacing, inBetween); ImGui::DragFloat(("##R" + label).c_str(), &values.r, speed); PopItemWidth(); ImGui::SameLine(); PopStyleVar();
		//
		//static const glm::vec4 Gcolor = { 0.161903f, 0.838096f, 0.161903f, 1.0f };
		//PushStyleColor(ImGuiCol_Button, *(ImVec4 *)&Gcolor);
		//PushStyleColor(ImGuiCol_ButtonHovered, *(ImVec4 *)&(Gcolor * phi<float>()));
		//PushStyleColor(ImGuiCol_ButtonActive, *(ImVec4 *)&(Gcolor * phi<float>() * phi<float>()));
		//PushStyleVar(ImGuiStyleVar_ItemSpacing, buttonToDrag);
		//if (Button(("G##" + label).c_str())) values.g = resetValue; PopStyleColor(3); ImGui::SameLine(); PopStyleVar(); PushStyleVar(ImGuiStyleVar_ItemSpacing, inBetween); ImGui::DragFloat(("##G" + label).c_str(), &values.g, speed); PopItemWidth(); ImGui::SameLine(); PopStyleVar();
		//
		//static const glm::vec4 Bcolor = { 0.161903f, 0.838096f, 0.161903f, 1.0f };
		//PushStyleColor(ImGuiCol_Button, *(ImVec4 *)&Bcolor);
		//PushStyleColor(ImGuiCol_ButtonHovered, *(ImVec4 *)&(Bcolor * phi<float>()));
		//PushStyleColor(ImGuiCol_ButtonActive, *(ImVec4 *)&(Bcolor * phi<float>() * phi<float>()));
		//PushStyleVar(ImGuiStyleVar_ItemSpacing, buttonToDrag);
		//if (Button(("B##" + label).c_str())) values.b = resetValue; PopStyleColor(3); ImGui::SameLine(); PopStyleVar(); PushStyleVar(ImGuiStyleVar_ItemSpacing, inBetween); ImGui::DragFloat(("##B" + label).c_str(), &values.b, speed); PopItemWidth(); ImGui::SameLine(); PopStyleVar();
		//
		//static const glm::vec4 Zcolor = { 0.161903f, 0.161903f, 0.838096f, 1.0f };
		//PushStyleColor(ImGuiCol_Button, *(ImVec4 *)&Zcolor);
		//PushStyleColor(ImGuiCol_ButtonHovered, *(ImVec4 *)&(Zcolor * phi<float>()));
		//PushStyleColor(ImGuiCol_ButtonActive, *(ImVec4 *)&(Zcolor * phi<float>() * phi<float>()));
		//PushStyleVar(ImGuiStyleVar_ItemSpacing, buttonToDrag);
		//if (Button(("A##" + label).c_str())) values.a = resetValue; PopStyleColor(3); ImGui::SameLine(); PopStyleVar(); PushStyleVar(ImGuiStyleVar_ItemSpacing, inBetween); ImGui::DragFloat(("##A" + label).c_str(), &values.a, speed); PopItemWidth();// ImGui::SameLine(); PopStyleVar();
		//
		//PushStyleVar(ImGuiStyleVar_ItemSpacing, buttonToDrag);
		//ColorEdit4(("##ColorEdit4_" + label).c_str(), glm::value_ptr(values));
		//PopStyleVar();
		PopID();
		Columns(1, label.c_str());
	}
	*/
	static void DrawFloat3Control(const std::string &label, glm::vec3 &values, float resetValue = 0.0f, float speed = 0.1f, float columnWidth = 100.0f)
	{
		using namespace ImGui;
		Columns(2);
		SetColumnWidth(0, (columnWidth > 0.0f ? columnWidth : m_fGlobalLabelsWidth));

		Text(label.c_str());

		NextColumn();
		PushMultiItemsWidths(3, CalcItemWidth());

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight * phi<float>(), lineHeight };
		static const ImVec2 inBetween = { phi<float>() * phi<float>() , 0.0f };
		static const ImVec2 buttonToDrag = { phi<float>(), 0.0f };

		static const glm::vec4 Xcolor = { 0.838096f, 0.161903f, 0.161903f, 1.0f };
		PushStyleColor(ImGuiCol_Button, *(ImVec4 *)&Xcolor);
		PushStyleColor(ImGuiCol_ButtonHovered, *(ImVec4 *)&(Xcolor * phi<float>()));
		PushStyleColor(ImGuiCol_ButtonActive, *(ImVec4 *)&(Xcolor * phi<float>() * phi<float>()));
		PushStyleVar(ImGuiStyleVar_ItemSpacing, buttonToDrag);
		if (Button(("X##" + label).c_str())) values.x = resetValue; PopStyleColor(3); ImGui::SameLine(); PopStyleVar(); PushStyleVar(ImGuiStyleVar_ItemSpacing, inBetween); ImGui::DragFloat(("##X" + label).c_str(), &values.x, speed); PopItemWidth(); ImGui::SameLine(); PopStyleVar();

		static const glm::vec4 Ycolor = { 0.161903f, 0.838096f, 0.161903f, 1.0f };
		PushStyleColor(ImGuiCol_Button, *(ImVec4 *)&Ycolor);
		PushStyleColor(ImGuiCol_ButtonHovered, *(ImVec4 *)&(Ycolor * phi<float>()));
		PushStyleColor(ImGuiCol_ButtonActive, *(ImVec4 *)&(Ycolor * phi<float>() * phi<float>()));
		PushStyleVar(ImGuiStyleVar_ItemSpacing, buttonToDrag);
		if (Button(("Y##" + label).c_str())) values.y = resetValue; PopStyleColor(3); ImGui::SameLine(); PopStyleVar(); PushStyleVar(ImGuiStyleVar_ItemSpacing, inBetween); ImGui::DragFloat(("##Y" + label).c_str(), &values.y, speed); PopItemWidth(); ImGui::SameLine(); PopStyleVar();

		static const glm::vec4 Zcolor = { 0.161903f, 0.161903f, 0.838096f, 1.0f };
		PushStyleColor(ImGuiCol_Button, *(ImVec4 *)&Zcolor);
		PushStyleColor(ImGuiCol_ButtonHovered, *(ImVec4 *)&(Zcolor * phi<float>()));
		PushStyleColor(ImGuiCol_ButtonActive, *(ImVec4 *)&(Zcolor * phi<float>() * phi<float>()));
		PushStyleVar(ImGuiStyleVar_ItemSpacing, buttonToDrag);
		if (Button(("Z##" + label).c_str())) values.z = resetValue; PopStyleColor(3); ImGui::SameLine(); PopStyleVar(); PushStyleVar(ImGuiStyleVar_ItemSpacing, inBetween); ImGui::DragFloat(("##Z" + label).c_str(), &values.z, speed); PopItemWidth(); PopStyleVar();
		Columns(1);
	}

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
				DrawFloat3Control("Position", cmp->Position);
				DrawFloat3Control("Rotation", cmp->Rotation);
				DrawFloat3Control("Scale", cmp->Scale, 1.0f);
				});
			DrawSelectionComponent<SpriteComponent>("Sprite", [](const Ref<Scene> &scene, const char *id, SpriteComponent *cmp) {
				//DrawColorControl("Tint Color: ", cmp->tint, 1.0f, 0.01f);
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
		color /= phi<float>();
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
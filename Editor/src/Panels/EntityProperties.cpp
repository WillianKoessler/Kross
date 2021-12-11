#include "Editor_pch.h"
#include "Kross/Util/Util.h"
#include "EntityProperties.h"
#include <glm/glm/gtc/type_ptr.hpp>

static constexpr float m_fGlobalLabelsWidth = 100.0f;
namespace Kross {
	
	static bool DrawColorControl(const std::string &label, glm::vec4 &values, float columnWidth = 100.0f)
	{
		using namespace ImGui;

		float lineHeight = GetFont()->FontSize + GetStyle().FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight * 1.61903f * 0.75f, lineHeight };
		//static constexpr float x = 0.424389f, o = 0.262126f;
		static constexpr float x = 0.262126f, o = 0.161903f;
		static constexpr glm::vec4 colors[4] = {
			{ x, o, o, 1.0f },
			{ o, x, o, 1.0f },
			{ o, o, x, 1.0f },
			{ x, x, x, 1.0f }
		};
		static const std::string fields[4] = { "R", "G", "B", "A" };

		PushID(label.c_str());
		ImGuiTableFlags flags = ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingStretchProp;
		glm::vec4 ranged = values * 255.f;
		int nVals[4] = { (int)ranged.r, (int)ranged.g, (int)ranged.b, (int)ranged.a };
		int column = 0;

		bool used = false;

		PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
		BeginTable(label.c_str(), 6, flags);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, columnWidth);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 80.9515f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 80.9515f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 80.9515f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 80.9515f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, buttonSize.x);
		TableNextRow();
		TableSetColumnIndex(column++);
		Text(label.c_str());
		float fVals[4];
		for (int i = 0; i < 4; i++)
		{
			TableSetColumnIndex(column++);
			PushItemWidth(GetContentRegionAvail().x);

			PushStyleColor(ImGuiCol_FrameBg, *(ImVec4 *)&colors[i]);
			PushStyleColor(ImGuiCol_FrameBgHovered, *(ImVec4 *)&(colors[i] * phi<float>()));
			PushStyleColor(ImGuiCol_FrameBgActive, *(ImVec4 *)&(colors[i] * phi<float>() * phi<float>()));
			used |= DragInt(("##" + fields[i] + label).c_str(), &nVals[i], 1, 0, 255, (fields[i] + ": %d").c_str(), ImGuiSliderFlags_AlwaysClamp);
			PopStyleColor(3);
			fVals[i] = nVals[i] / 255.f;
		}

		ImVec4 ImGuiValues = { fVals[0], fVals[1], fVals[2], fVals[3] };
		TableSetColumnIndex(column++);
		if (used |= ColorButton("##ColorButton", ImGuiValues, ImGuiColorEditFlags_AlphaPreviewHalf))
		{
			OpenPopupOnItemClick("picker", ImGuiPopupFlags_MouseButtonLeft);
			SetNextWindowPos(*(ImVec2 *)&Input::GetMousePosition(), ImGuiCond_Always);
		}
		if (BeginPopup("picker"))
		{
			used |= ColorPicker4("##picker", fVals);
			EndPopup();
		}
		PopStyleVar();
		EndTable();
		PopID();
		values = { fVals[0], fVals[1], fVals[2], fVals[3] };
		return used;
	}
	static bool DrawFloat3Control(const std::string &label, glm::vec3 &values, float resetValue = 0.0f, float speed = .01f, float columnWidth = 100.0f)
	{
		using namespace ImGui;

		std::string format = "%.";
		int max_decimal_places = 5;
		{
			static std::string buffer;
			buffer = std::to_string(speed);
			buffer = buffer.substr(buffer.find_first_of('.')+1, max_decimal_places);
			int discount = 0;
			for (size_t i = buffer.size() - 1; i >= 0; i--)
				if (buffer[i] == '0') discount++;
				else break;
			format += std::to_string(max_decimal_places-discount) + "f";
		}

		float lineHeight = GetFont()->FontSize + GetStyle().FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight * 1.61903f * 0.5f, lineHeight };
		static constexpr float x = 0.838096f, o = 0.161903f;
		static constexpr glm::vec4 colors[3] = {
			{ x, o, o, 1.0f },
			{ o, x, o, 1.0f },
			{ o, o, x, 1.0f },
		};
		static const std::string axis[3] = { "X", "Y", "Z" };
		
		PushID(label.c_str());
		ImGuiTableFlags flags = ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingStretchProp;
		float fvalues[3] = { values.x, values.y, values.z };
		int column = 0;

		bool used = false;

		PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(.5f, 1));
		BeginTable(label.c_str(), 7, flags);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, columnWidth);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, buttonSize.x);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 83.8096f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, buttonSize.x);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 83.8096f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, buttonSize.x);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 83.8096f);
		TableNextRow();
		TableSetColumnIndex(column++);
		Text(label.c_str());
		for (int i = 0; i < 3; i++)
		{
			TableSetColumnIndex(column++);
			PushStyleColor(ImGuiCol_Button, *(ImVec4 *)&colors[i]);
			PushStyleColor(ImGuiCol_ButtonHovered, *(ImVec4 *)&(colors[i] * phi<float>()));
			PushStyleColor(ImGuiCol_ButtonActive, *(ImVec4 *)&(colors[i] * phi<float>() * phi<float>()));
			PushItemWidth(ImGui::GetContentRegionAvail().x);
			if (used |= Button((axis[i] + "##" + label).c_str()))
				fvalues[i] = resetValue;
			PopStyleColor(3);

			TableSetColumnIndex(column++);
			PushItemWidth(ImGui::GetContentRegionAvail().x);
			used |= DragFloat(("##" + axis[i] + label).c_str(), &fvalues[i], speed, 0.0f, 0.0f, format.c_str(), ImGuiSliderFlags_NoRoundToFormat);
		}
		PopStyleVar();
		EndTable();
		PopID();

		values = { fvalues[0], fvalues[1], fvalues[2] };
		return used;
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
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(ImGui::GetStyle().ItemSpacing.x, 0.0f));
				DrawFloat3Control("Position", cmp->Position);
				DrawFloat3Control("Rotation", cmp->Rotation);
				DrawFloat3Control("Scale", cmp->Scale, 1.0f);
				ImGui::PopStyleVar();
				});
			DrawSelectionComponent<SpriteComponent>("Sprite", [](const Ref<Scene> &scene, const char *id, SpriteComponent *cmp) {
				DrawColorControl("Tint", cmp->tint);
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
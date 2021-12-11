#include "Editor_pch.h"
#include "Kross/Util/Util.h"
#include "EntityProperties.h"
#include <glm/glm/gtc/type_ptr.hpp>

static constexpr float x = 0.838096f, o = 0.161903f;
static constexpr glm::vec4 colors[4] = {
	{ x, o, o, 1.0f },
	{ o, x, o, 1.0f },
	{ o, o, x, 1.0f },
	{ x, x, x, 1.0f }
};
//static const std::string axis[3] = { "X", "Y", "Z" };
static const char *axis[3] = { "X", "Y", "Z" };
static const std::string channel[4] = { "R", "G", "B", "A" };

namespace Kross {
	static void PushColor(const glm::vec4 &color, int normal, int hovered = 0, int active = 0)
	{
		ImGui::PushStyleColor(normal, *(ImVec4 *)&color);
		ImGui::PushStyleColor(hovered, *(ImVec4 *)&(color * phi<float>()));
		ImGui::PushStyleColor(active, *(ImVec4 *)&(color * phi<float>() * phi<float>()));
	}
	static void PopColor(int count = 1)
	{
		ImGui::PopStyleColor(count);
	}
	// turns 1234.1234f into "%.4f"
	static const std::string GetFloatFormat(float value, uint8_t max = 6, uint8_t min = 1)
	{
		static std::string buffer;
		buffer = std::to_string(value);
		buffer = buffer.substr(1 + buffer.find_first_of('.'), max > min ? max : min);
		int discount = 0;
		for (int i = (int)buffer.size() - 1; i > -1; i--) {
			if (buffer[i] != '0') break;
			else discount++;
		}
		int result = max - discount;
		return "%." + std::to_string((result > min ? result : min)) + "f";
	}
	// turns 1234.1234f into 0.0001f
	static float GetSpeed(float value, float limit = 0.00001f)
	{
		if (value == 0.0f) return 0.1f;
		static std::string buffer;
		buffer = std::to_string(value); // "xxx.xxx"
		buffer = buffer.substr(buffer.find_first_of('.') + 1); // xxx."xxx"
		float speed = 1.0f;
		for (int i = (int)buffer.size() - 1; i > -1; i--)
			if (buffer[i] != '0') speed *= 0.1f;
		if (speed < limit) return limit;
		else return speed;
	}
	static bool DrawScalarN(const char *id, float *vec, uint8_t n, float reset_value, float speed)
	{
		using namespace ImGui;
		PushID(id);
		bool value_changed = false;
		float lineHeight = GetFont()->FontSize + GetStyle().FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight / phi<float>(), lineHeight };
		float dragWidth = (GetContentRegionAvail().x - (buttonSize.x * n)) / n;
		for (uint8_t i = 0; i < n; i++)
		{
			PushID(i);
			if (i != 0) SameLine(0.0f, 0.0f);
			PushItemWidth(buttonSize.x);
			PushColor(colors[i], ImGuiCol_Button, ImGuiCol_ButtonHovered, ImGuiCol_ButtonActive);
			if (Button(axis[i])) vec[i] = reset_value;
			PopColor(3);
			PopItemWidth();

			SameLine(0.0f, 0.0f);
			PushItemWidth(dragWidth);
			PushColor(colors[i] * o, ImGuiCol_FrameBg, ImGuiCol_FrameBgHovered, ImGuiCol_FrameBgActive);
			DragFloat("", &vec[i], speed, .0f, .0f, GetFloatFormat(speed, 6).c_str());
			value_changed |= IsItemFocused();

			PopColor(3);
			PopItemWidth();
			PopID();
		}
		PopID();
		return value_changed;
	}
	static bool DrawVec3(const char *id, glm::vec3 &vec, float speed = 0.1f, float reset_value = 0.0f)
	{
		return DrawScalarN(id, glm::value_ptr(vec), 3, reset_value, speed);
	}
	static bool DrawColorControl(const std::string &ID, const std::string &label, glm::vec4 &values, float columnWidth = 100.0f)
	{
		using namespace ImGui;

		float lineHeight = GetFont()->FontSize + GetStyle().FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight * 1.61903f * 0.75f, lineHeight };

		std::string id = label + "_" + ID;

		PushID(id.c_str());
		ImGuiTableFlags flags = ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingStretchProp;
		glm::vec4 ranged = values * 255.f;
		int nVals[4] = { (int)ranged.r, (int)ranged.g, (int)ranged.b, (int)ranged.a };
		int column = 0;

		bool used = false;

		PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
		BeginTable(id.c_str(), 6, flags);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, columnWidth);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 80.9515f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 80.9515f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 80.9515f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 80.9515f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, buttonSize.x);
		TableNextRow();
		TableSetColumnIndex(column++);
		Text((label + " ##Label_" + id).c_str());
		float fVals[4];
		for (int i = 0; i < 4; i++)
		{
			TableSetColumnIndex(column++);
			PushItemWidth(GetContentRegionAvail().x);

			PushStyleColor(ImGuiCol_FrameBg, *(ImVec4 *)&colors[i]);
			PushStyleColor(ImGuiCol_FrameBgHovered, *(ImVec4 *)&(colors[i] * phi<float>()));
			PushStyleColor(ImGuiCol_FrameBgActive, *(ImVec4 *)&(colors[i] * phi<float>() * phi<float>()));
			used |= DragInt(("##" + channel[i] + id).c_str(), &nVals[i], 1, 0, 255, (channel[i] + ": %d").c_str(), ImGuiSliderFlags_AlwaysClamp);
			PopStyleColor(3);
			fVals[i] = nVals[i] / 255.f;
		}

		ImVec4 ImGuiValues = { fVals[0], fVals[1], fVals[2], fVals[3] };
		TableSetColumnIndex(column++);
		if (used |= ColorButton(("##ColorButton_" + id).c_str(), ImGuiValues, ImGuiColorEditFlags_AlphaPreviewHalf))
		{
			OpenPopupOnItemClick("picker", ImGuiPopupFlags_MouseButtonLeft);
			SetNextWindowPos(*(ImVec2 *)&Input::GetMousePosition(), ImGuiCond_Always);
		}
		if (BeginPopup("picker"))
		{
			used |= ColorPicker4(("##ColorPicker_" + id).c_str(), fVals);
			EndPopup();
		}
		PopStyleVar();
		EndTable();
		PopID();
		values = { fVals[0], fVals[1], fVals[2], fVals[3] };
		return used;
	}
	static bool DrawFloat3Control(const std::string &id, glm::vec3 &values, float resetValue = 0.0f, float speed = .01f, float columnWidth = 100.0f)
	{
		using namespace ImGui;


		float lineHeight = GetFont()->FontSize + GetStyle().FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight * 1.61903f * 0.5f, lineHeight };
		PushID(id.c_str());
		ImGuiTableFlags flags = ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingStretchProp;
		float fvalues[3] = { values.x, values.y, values.z };
		int column = 0;

		bool used = false;

		PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(.5f, 1));
		BeginTable(id.c_str(), 6, flags);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, buttonSize.x);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, x * 100.0f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, buttonSize.x);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, x * 100.0f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, buttonSize.x);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, x * 100.0f);
		TableNextRow();
		TableNextColumn();
		for (int i = 0; i < 3; i++)
		{
			TableNextColumn();
			PushStyleColor(ImGuiCol_Button, *(ImVec4 *)&colors[i]);
			PushStyleColor(ImGuiCol_ButtonHovered, *(ImVec4 *)&(colors[i] * phi<float>()));
			PushStyleColor(ImGuiCol_ButtonActive, *(ImVec4 *)&(colors[i] * phi<float>() * phi<float>()));
			if (used |= Button((axis[i] + ("##" + id)).c_str()))
				fvalues[i] = resetValue;
			PopStyleColor(3);

			TableNextColumn();
			used |= DragFloat(("##" + (axis[i] + id)).c_str(), &fvalues[i], speed, 0.0f, 0.0f, GetFloatFormat(speed, 5).c_str(), ImGuiSliderFlags_NoRoundToFormat);
		}
		PopStyleVar();
		EndTable();
		PopID();

		values = { fvalues[0], fvalues[1], fvalues[2] };
		return used;
	}
	template<typename Component>
	static void DrawComponent(const std::string &label, Entity entity, void(*show)(Scene *, const std::string &, Component *))
	{
		if (!entity.GetScene()) return;
		static ImGuiTableFlags flags =
			ImGuiTableFlags_NoHostExtendX |
			ImGuiTableFlags_SizingStretchProp |
			ImGuiTableFlags_Borders |
			ImGuiTableFlags_NoPadInnerX |
			ImGuiTableFlags_NoPadOuterX;
		if (entity.Has<Component>() == 1) {
			if (ImGui::TreeNodeEx((const void *)(typeid(Component).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, label.c_str())) {
				ImGui::BeginTable(("##" + label).c_str(), 2, flags);
				ImGui::TableSetupColumn(("##" + label + "_Label").c_str(), ImGuiTableColumnFlags_WidthFixed, 100.0f);
				ImGui::TableSetupColumn(("##" + label + "_Value").c_str(), ImGuiTableColumnFlags_WidthStretch, ImGui::GetContentRegionAvail().x);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				show(const_cast<Scene *>(entity.GetScene()), "##" + label, entity.Get<Component>());
				ImGui::EndTable();
				ImGui::Separator();
				ImGui::TreePop();
			}
		}
	}
	static void PushDisabled(int item, int hovered, int activated)
	{
		glm::vec4 color = *(glm::vec4 *)&ImGui::GetStyleColorVec4(item);
		color /= phi<float>();
		if (item > -1 && item < ImGuiCol_COUNT) ImGui::PushStyleColor(item, *(ImVec4 *)&color);
		if (hovered > -1 && hovered < ImGuiCol_COUNT) ImGui::PushStyleColor(hovered, *(ImVec4 *)&color);
		if (activated > -1 && activated < ImGuiCol_COUNT) ImGui::PushStyleColor(activated, *(ImVec4 *)&color);
	}
	static void PopDisabled() { ImGui::PopStyleColor(3); }

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
		{
			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(.5f, 1));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(ImGui::GetStyle().ItemSpacing.x, 0.0f));
			DrawEntity(s_Selection);
			ImGui::PopStyleVar(2);
		}
		ImGui::End();
	}
	void EntityProperties::DrawEntity(Entity &entity)
	{
		if (entity && entity.GetScene()) {
			DrawComponent<TagComponent>("Tag", entity, [](Scene *scene, const std::string &id, TagComponent *cmp) {
				char buffer[TagComponent::limit];
				memset(buffer, 0, TagComponent::limit);
				strcpy_s(buffer, TagComponent::limit, cmp->tag);
				ImGui::Text("Name: ");
				ImGui::TableNextColumn();
				ImGuiInputTextFlags flags = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::InputText(id.c_str(), buffer, TagComponent::limit, flags))
					strcpy_s(cmp->tag, TagComponent::limit, buffer);
				});
			DrawComponent<TransformComponent>("Transform", entity, [](Scene *scene, const std::string &id, TransformComponent *cmp) {
				bool active = false;
				ImGui::Text("Position:"); ImGui::TableNextColumn(); active |= DrawVec3("##Position", cmp->Position, 0.001f); ImGui::TableNextColumn();
				ImGui::Text("Rotation:"); ImGui::TableNextColumn(); active |= DrawVec3("##Rotation", cmp->Rotation, 0.001f); ImGui::TableNextColumn();
				ImGui::Text("Scale:");    ImGui::TableNextColumn(); active |= DrawVec3("##Scale", cmp->Scale, 0.001f);
				if (Input::IsMouseButtonHeld(MouseButton::Left)) {
					setFlag(ImGuiConfigFlags_NoMouse, active);
					Application::Get().GetWindow().CursorEnabled(!active);
				} else {
					setFlag(ImGuiConfigFlags_NoMouse, false);
					Application::Get().GetWindow().CursorEnabled(true);
				}
				});
			DrawComponent<SpriteComponent>("Sprite", entity, [](Scene *scene, const std::string &id, SpriteComponent *cmp) {
				DrawColorControl("Sprite", "Tint", cmp->tint);
				});
			DrawComponent<CameraComponent>("Camera", entity, [](Scene *scene, const std::string &id, CameraComponent *cmp) {
				SceneCamera &camera = cmp->camera;
				Entity activeCamera = scene->GetCurrentCamera();

				ImGui::PushID("Camera");
				ImGuiTableFlags flags =
					ImGuiTableFlags_NoHostExtendX |
					ImGuiTableFlags_SizingStretchProp |
					ImGuiTableFlags_Borders;
				ImGui::BeginTable("Camera_Table", 2, flags);
				ImGui::TableSetupColumn("Camera_Table_Labels", ImGuiTableColumnFlags_WidthFixed, 100.0f);
				ImGui::TableSetupColumn("Camera_Table_Contents", ImGuiTableColumnFlags_WidthStretch, ImGui::GetContentRegionAvail().x);
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Active Camera: ");
					ImGui::TableNextColumn();

					if (activeCamera) {
						if (activeCamera.Has<TagComponent>() == 1) ImGui::Text("(%s)", activeCamera.Get<TagComponent>()->tag);
						else ImGui::Text("(%d)", (uint32_t)activeCamera);
					} else ImGui::Text("(null)");
				}
				{
					ImGui::TableNextColumn();
					ImGui::Text("Activate Camera: ");
					ImGui::TableNextColumn();
					if (s_Selection) {
						bool activate = false;
						ImGui::BeginDisabled(s_Selection.Has<TagComponent, TransformComponent>() != 1);
						if (activeCamera) activate = s_Selection == activeCamera;
						if (ImGui::Checkbox("##Activate Camera", &activate))
							scene->SetPrimaryCamera(s_Selection);
						ImGui::EndDisabled();
					}
				}
				ImGui::EndTable();


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
				ImGui::PopID();
				});
		}
	}
}

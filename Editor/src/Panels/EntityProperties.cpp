#include "Editor_pch.h"
#include "Kross/Util/Util.h"
#include "EntityProperties.h"
#include <glm/glm/gtc/type_ptr.hpp>
static bool debug = false;
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
	static const std::string GetFloatFormat(float val1, float val2, uint8_t max = 6, uint8_t min = 1)
	{
		static std::string buffer;
		int r[2] = { 0, 0 };
		float vals[2] = { val1, val2 };
		bool zero[2] = { false };
		uint8_t _max, _min;
		if (max > min) {
			_max = max; _min = min;
		} else { _max = min; _min = max; }
		for (int i = 0; i < 2; i++)
		{
			buffer = std::to_string(vals[i]);
			auto dot = buffer.find_first_of('.') + 1;
			buffer = buffer.substr(dot, dot + _max);
			int discount = 0;
			int size = (int)buffer.size();
			for (int i = size - 1; i > -1; i--) {
				if (buffer[i] != '0') break;
				else discount++;
			}
			int n = size - discount;
			zero[i] = n == 0;
			r[i] = std::clamp<int>(n, _min, _max);
		}
		if (!zero[0]) return "%." + std::to_string((r[0] > r[1] ? r[0] : r[1])) + "f";
		else return "%.0f";
	}
	static const std::string GetFloatFormat(float value, uint8_t max = 6, uint8_t min = 1)
	{
		static std::string buffer;
		int _max, _min; if (max > min) { _max = max; _min = min; } else { _max = min; _min = max; }
		buffer = std::to_string(value);
		if (false)
		{
			static int column = 0, rows = 0;;
			column++;
			printf("%s\t", buffer.c_str());
			if (column > 2) {
				column = 0;
				printf("\n");
				rows++;
			}
			if (rows > 2) {
				rows = 0;
				printf("\n");
			}
		}
		int dot = (int)buffer.find_first_of('.') + 1;
		buffer = buffer.substr(dot, (size_t)(dot + _max));
		int discount = 0;
		int size = (int)buffer.size();
		for (int i = size - 1; i > -1; i--) {
			if (buffer[i] != '0') break;
			else discount++;
		}
		return "%." + std::to_string(std::clamp<int>(size - discount, _min, _max)) + "f";
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
			DragFloat("", &vec[i], speed, .0f, .0f, GetFloatFormat(speed).c_str());
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

	static bool DrawColorControl(const char *id, float *values)
	{
		using namespace ImGui;

		float pickerWidth = GetFont()->FontSize + GetStyle().FramePadding.y * 2.428545f;
		float dragWidth = (GetContentRegionAvail().x - pickerWidth) / 4;
		PushID(id); // push 1
		bool value_changed = false;

		for (int i = 0; i < 4; i++) {
			PushID(i); // push 2 3 4 5
			PushItemWidth(dragWidth);
			int val = (int)(values[i] * 255.f);
			if (i != 3)  PushColor(colors[i] * x * values[i], ImGuiCol_FrameBg, ImGuiCol_FrameBgHovered, ImGuiCol_FrameBgActive);
			else PushColor(colors[i] * o * values[i], ImGuiCol_FrameBg, ImGuiCol_FrameBgHovered, ImGuiCol_FrameBgActive);
			DragInt(("##" __FUNCSIG__ + channel[i] + id).c_str(), &val, 1, 0, 255, (channel[i] + ": %d").c_str(), ImGuiSliderFlags_AlwaysClamp); SameLine(0.0f, 0.0f);
			PopColor(3);
			value_changed |= IsItemFocused();
			values[i] = val / 255.f;
			PopID(); // pop 5 4 3 2
		}
		{
			ImVec4 ImGuiValues = { values[0], values[1], values[2], values[3] };
			PushItemWidth(pickerWidth);
			if (ColorButton("##ColorButton_" __FUNCSIG__, ImGuiValues, ImGuiColorEditFlags_AlphaPreviewHalf)) {
				OpenPopupOnItemClick("picker" __FUNCSIG__, ImGuiPopupFlags_MouseButtonLeft);
			}
			if (BeginPopup("picker" __FUNCSIG__)) {
				ColorPicker4("##ColorPicker_" __FUNCSIG__, values);
				EndPopup();
			}
		}
		PopID(); // pop 1
		return value_changed;
	}

	template<typename Component>
	static void DrawComponent(const std::string &label, Entity entity, bool removable, void(*show)(Scene *, const std::string &, Component *))
	{
		if (!entity.GetScene()) return;

		ImGuiTableFlags tableFlags = ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingStretchProp |
			ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX | (debug ? ImGuiTableFlags_Borders : 0);

		ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		if (entity.Has<Component>() == 1) {
			ImGui::PushID(("##" + label).c_str());
			float totalWidth = ImGui::GetContentRegionAvail().x;
			bool markForDelete = false;
			bool opened = ImGui::TreeNodeEx((const void *)(typeid(Component).hash_code()), treeFlags, label.c_str());
			if (removable) {
				//				ImGui::SameLine(0.0f, totalWidth - labelWidth - buttonWidth - (opened ? 0.0f : buttonWidth));

				if (ImGui::BeginPopupContextItem("RemoveComponentPopup")) {
					markForDelete |= ImGui::MenuItem("Remove Component");
					ImGui::EndPopup();
				}

				ImGui::SameLine(totalWidth - ImGui::CalcTextSize("...").x);
				if (ImGui::SmallButton("..."))
					ImGui::OpenPopup("ComponentSettings");

				if (ImGui::BeginPopup("ComponentSettings")) {
					markForDelete |= ImGui::MenuItem("Remove Component");
					ImGui::EndPopup();
				}
			}

			if (opened) {
				ImGui::BeginTable(("##" + label).c_str(), 2, tableFlags);
				ImGui::TableSetupColumn(("##" + label + "_Label").c_str(), ImGuiTableColumnFlags_WidthFixed, 100.0f);
				ImGui::TableSetupColumn(("##" + label + "_Value").c_str(), ImGuiTableColumnFlags_WidthStretch, ImGui::GetContentRegionAvail().x);
				ImGui::TableNextRow(); ImGui::TableNextColumn();
				show(const_cast<Scene *>(entity.GetScene()), "##" + label, entity.Get<Component>());
				ImGui::EndTable();
				ImGui::TreePop();
			}
			ImGui::Separator();
			ImGui::PopID();
			if (markForDelete) entity.Remove<Component>();
		}
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

		if (ImGui::Begin(m_strName, &Panel::Manager().s_bPropertiesInspector))
		{
			if (s_Selection) {
				if (s_Selection.Has<TagComponent>() == 1) {
					bool hasTransform = s_Selection.Has<TransformComponent>() == 1;
					bool hasSprite = s_Selection.Has<SpriteComponent>() == 1;
					bool hasCamera = s_Selection.Has<CameraComponent>() == 1;
					bool hasAll = hasTransform && hasSprite && hasCamera;

					float buttonWidth = 0.0f;
					if (!hasAll) buttonWidth = ImGui::CalcTextSize("Add Component__").x;
					{
						auto cmp = s_Selection.Get<TagComponent>();
						char buffer[TagComponent::limit];
						memset(buffer, 0, TagComponent::limit);
						strcpy_s(buffer, TagComponent::limit, cmp->Get());
						ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
						ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - buttonWidth);
						if (ImGui::InputText("##Tag", buffer, TagComponent::limit, flags))
							cmp->Set(buffer);
					}


					if (!hasAll) {
						ImGui::SameLine();
						ImGui::PushItemWidth(-1.0f);
						if (ImGui::Button("Add Component"))
							ImGui::OpenPopup("Add_Component_Popup");
						if (ImGui::BeginPopup("Add_Component_Popup")) {
							if (!hasTransform && ImGui::MenuItem("Transform")) { s_Selection.Add<TransformComponent>(); ImGui::CloseCurrentPopup(); }
							if (!hasSprite && ImGui::MenuItem("Sprite")) { s_Selection.Add<SpriteComponent>(); ImGui::CloseCurrentPopup(); }
							if (!hasCamera && ImGui::MenuItem("Camera")) { s_Selection.Add<CameraComponent>(); ImGui::CloseCurrentPopup(); }
							ImGui::EndPopup();
						}
					}
				}
				DrawEntity(s_Selection);
			}
		}
		ImGui::End();
	}
	void EntityProperties::DrawEntity(Entity &entity)
	{
		if (entity && entity.GetScene()) {
			DrawComponent<TransformComponent>("Transform", entity, true, [](Scene *scene, const std::string &id, TransformComponent *cmp) {
				bool active = false;
				ImGui::Text("Position:"); ImGui::TableNextColumn(); active |= DrawVec3("##Position", cmp->Position, 0.01f); ImGui::TableNextColumn();
				ImGui::Text("Rotation:"); ImGui::TableNextColumn(); active |= DrawVec3("##Rotation", cmp->Rotation, 0.01f); ImGui::TableNextColumn();
				ImGui::Text("Scale:");    ImGui::TableNextColumn(); active |= DrawVec3("##Scale", cmp->Scale, 0.01f, 1.0f);
				if (Input::IsMouseButtonHeld(MouseButton::Left)) {
					setFlag(ImGuiConfigFlags_NoMouse, active);
					Application::Get().GetWindow().CursorEnabled(!active);
				} else {
					setFlag(ImGuiConfigFlags_NoMouse, false);
					Application::Get().GetWindow().CursorEnabled(true);
				}
				});
			DrawComponent<SpriteComponent>("Sprite", entity, true, [](Scene *scene, const std::string &id, SpriteComponent *cmp) {
				ImGui::Text("Tint"); ImGui::TableNextColumn(); DrawColorControl("Sprite_Tint", glm::value_ptr(cmp->tint));
				});
			DrawComponent<CameraComponent>("Camera", entity, true, [](Scene *scene, const std::string &id, CameraComponent *cmp) {
				SceneCamera &camera = cmp->camera;
				Entity activeCamera = scene->GetCurrentCamera();
				{
					ImGui::Text("Active Camera: "); ImGui::TableNextColumn();

					if (s_Selection) {
						bool disable = s_Selection.Has<TagComponent, TransformComponent>() != 1;
						ImGui::BeginDisabled(disable);
						if (ImGui::Button("SET##Activate Camera"))
							scene->SetPrimaryCamera(s_Selection);
						ImGui::EndDisabled();
						if (disable) {
							ImGui::SameLine();
							Panel::ShowHelperMarker("This Entity does not have a TagComponent or a TransformComponent");
						}
						ImGui::SameLine();
					}

					if (activeCamera) {
						if (activeCamera.Has<TagComponent>() == 1) ImGui::Text("(Current: %s)", activeCamera.Get<TagComponent>()->Get());
						else ImGui::Text("(Current: %d)", (uint32_t)activeCamera);
					} else ImGui::Text("(Current: null)");
					ImGui::TableNextColumn();
				}
				{
					static constexpr char *projTypeStr[] = { "Perspective", "Orthographic" };
					const char *selection = projTypeStr[(int)cmp->camera.GetProjType()];
					ImGui::Text("Projection: "); ImGui::TableNextColumn();
					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
					if (ImGui::BeginCombo("##Projection", selection, ImGuiComboFlags_NoArrowButton))
					{
						for (int i = 0; i < 2; i++) {
							bool match = selection == projTypeStr[i];
							if (ImGui::Selectable(projTypeStr[i], match)) {
								selection = projTypeStr[i];
								camera.SetProjType((SceneCamera::ProjectionType)i);
							}
							if (match) ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
					ImGui::TableNextColumn();
				}
				{
					switch (cmp->camera.GetProjType()) {
						default: ImGui::Text("Invalid Camera Projection Type"); break;
						case SceneCamera::ProjectionType::Orthographic:
							{
								float fSize = camera.OrthoSize();
								float fNear = camera.GetNearClip();
								float fFar = camera.GetFarClip();
								ImGui::Text("OrthoSize: "); ImGui::TableNextColumn();
								if (ImGui::DragFloat("##OrthographicOrthoSize: ", &fSize, 0.1f)) camera.SetOrthoSize(fSize); ImGui::TableNextColumn();
								ImGui::Text("NearClip: "); ImGui::TableNextColumn();
								if (ImGui::DragFloat("##OrthographicNearClip: ", &fNear, 0.1f)) camera.SetNearClip(fNear); ImGui::TableNextColumn();
								ImGui::Text("FarClip: "); ImGui::TableNextColumn();
								if (ImGui::DragFloat("##OrthographicFarClip: ", &fFar, 0.1f)) camera.SetFarClip(fFar); ImGui::TableNextColumn();
								break;
							}
						case SceneCamera::ProjectionType::Perspective:
							{
								float fFOV = glm::degrees(camera.GetPerspVerticalFOV());
								float fNear = camera.GetNearClip();
								float fFar = camera.GetFarClip();
								ImGui::Text("FOV: "); ImGui::TableNextColumn();
								if (ImGui::DragFloat("##PerspectiveFOV: ", &fFOV, 0.1f)) camera.SetPerspVerticalFOV(glm::radians(fFOV)); ImGui::TableNextColumn();
								ImGui::Text("NearClip: "); ImGui::TableNextColumn();
								if (ImGui::DragFloat("##PerspectiveNearClip: ", &fNear, 0.1f)) camera.SetNearClip(fNear); ImGui::TableNextColumn();
								ImGui::Text("FarClip: "); ImGui::TableNextColumn();
								if (ImGui::DragFloat("##PerspectiveFarClip: ", &fFar, 0.1f)) camera.SetFarClip(fFar); ImGui::TableNextColumn();
								break;
							}
					}
					ImGui::Text("Fixed AR"); ImGui::TableNextColumn();
					ImGui::BeginDisabled(cmp->camera.GetProjType() != SceneCamera::ProjectionType::Orthographic);
					ImGui::Checkbox("##Fixed Aspect Ratio", &cmp->fixedAspectRatio);
					ImGui::EndDisabled();
				}
				});
		}
	}
}

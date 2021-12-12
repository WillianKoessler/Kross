#include "Editor_pch.h"
#include "SceneHierarchy.h"

namespace Kross {
	SceneHierarchy::SceneHierarchy(const Ref<Scene> &scene)
		: p_Scene(scene)
	{
		m_strName = "Scene Inspector";
		m_Flags |= ImGuiWindowFlags_AlwaysAutoResize;
		if (!scene) KROSS_WARN("Scene supplied is nullptr");
		KROSS_INFO("Panel '{0}' Constructed", m_strName);
	}
	void SceneHierarchy::SetContext(const Ref<Scene> &scene)
	{
		if (scene) p_Scene = scene;
	}
	void SceneHierarchy::Show(double ts)
	{
		if (!Manager().s_bSceneHierarchy) return;
		if (ImGui::Begin(m_strName, &Manager().s_bSceneHierarchy) && p_Scene)
		{
			auto all = p_Scene->GetAllEntities();
			for (Entity &entity : all) DrawEntityNode(entity);

			if (Input::IsMouseButtonPressed(MouseButton::Left) && ImGui::IsWindowHovered())
				s_Selection = {};

			if (ImGui::BeginPopupContextWindow("SceneHierarchy_Popup", 1, false))
			{
				if (ImGui::MenuItem("New Entity")) p_Scene->CreateEntity("New Entity");
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
	void SceneHierarchy::DrawEntityNode(Entity &entity)
	{
		if (entity.Has<TagComponent>() == 1) {
			auto tc = entity.Get<TagComponent>();
			ImGuiTreeNodeFlags flags = (s_Selection == entity) ? ImGuiTreeNodeFlags_Selected : 0;
			bool opened = ImGui::TreeNodeEx((const void *)(uint64_t)ImGui::GetID(tc->Get()), flags, tc->Get());
			if (ImGui::IsItemClicked()) s_Selection = entity;
			bool markToDelete = false;
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Delete Entity"))
					markToDelete = true;
				ImGui::EndPopup();
			}
			if (opened)
				ImGui::TreePop();

			if (markToDelete) {
				if (s_Selection == entity) s_Selection = {};
				p_Scene->DestroyEntity(entity);
			}
		}
	}
}
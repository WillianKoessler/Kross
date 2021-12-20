#include "Editor_pch.h"
#include "SceneHierarchy.h"
#include "Panel.h"

namespace Kross {
	SceneHierarchy::SceneHierarchy(Scene &scene)
		: p_Scene(&scene)
	{
		m_strName = "Scene Inspector";
		m_Flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_Popup;
		if (!p_Scene) KROSS_WARN("Scene supplied is nullptr");
		KROSS_INFO("Panel '{0}' Constructed", m_strName);
	}
	void SceneHierarchy::Show()
	{
		if (!Panel::Manager().s_bSceneHierarchy) return;
		if (ImGui::Begin(m_strName, &Panel::Manager().s_bSceneHierarchy) && p_Scene)
		{
			auto all = p_Scene->GetAllEntities();
			for (Entity &entity : all) DrawEntityNode(entity);

			if (Input::IsMouseButtonPressed(MouseButton::Left) && ImGui::IsWindowHovered())
				p_Scene->ClearSelection();

			if (ImGui::BeginPopupContextWindow("SceneHierarchy_Popup", 1, false))
			{
				if (ImGui::MenuItem("New Entity")) p_Scene->CreateEntity("New Entity");
				ImGui::EndPopup();
			}

			if (Input::IsKeyHeld(Key::LeftControl)) {
				if (p_Scene->Selected())
					if (Input::IsKeyPressed(Key::C))
						m_Clipboard = p_Scene->Selected();
				if (Input::IsKeyPressed(Key::V)) {
					if (m_Clipboard)
						p_Scene->CreateEntity(m_Clipboard);
				}
			}
		}
		ImGui::End();
	}
	void SceneHierarchy::DrawEntityNode(Entity &entity)
	{
		if (entity.Has<TagComponent>() == 1) {
			auto tc = entity.Get<TagComponent>();
			ImGuiTreeNodeFlags flags = (p_Scene->Selected() == entity) ? ImGuiTreeNodeFlags_Selected : 0;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((const void *)(uint64_t)ImGui::GetID(tc->Get()), flags, "%s (id: %lu)", tc->Get(), (uint32_t)entity);
			if (ImGui::IsItemClicked()) p_Scene->Select(entity);
			bool markForDelete = false;

			if (entity == p_Scene->Selected() && Input::IsKeyPressed(Key::Delete))
				markForDelete = true;

			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Delete Entity"))
					markForDelete = true;
				ImGui::EndPopup();
			}

			if (opened)
				ImGui::TreePop();

			if (markForDelete) {
				if (m_Clipboard == entity) m_Clipboard = {};
				p_Scene->DestroyEntity(entity);
			}
		}
	}
}

#pragma once

#include "Panel.h"

namespace Kross {
	class EntityProperties : public Panel
	{
	public:
		EntityProperties(const Ref<Scene>& scene);
		void Show(double ts) override;
		void DrawEntity(Entity &entity);
	private:
		template<typename Component> void DrawComponent(Entity &entity, const char* label, void(*show)(const char* id, Component*))
		{
			static std::string emptyID("");
			if (entity.Has<Component>() == 1) {
				ImGui::Text(label); ImGui::SameLine();
				show(emptyID.append(" ").c_str(), entity.Get<Component>());
			}
		}
		template<typename Component> void DrawSelectionComponent(const char* label, void(*show)(const char* id, Component*))
		{
			static std::string emptyID("");
			if (s_Selection.Has<Component>() == 1) {
				ImGui::Text(label); ImGui::SameLine();
				show(emptyID.append(" ").c_str(), s_Selection.Get<Component>());
				ImGui::Separator();
			}
		}
	private:
		Ref<Scene> p_Scene;
	};
}
#pragma once

#include "Panel.h"

namespace Kross {
	class EntityProperties : public Panel
	{
	public:
		EntityProperties(const Ref<Scene>& scene);
		virtual void Show(double ts) override;
		void DrawEntity(Entity &entity);
	private:
		static void SetDisabledStyle(int item, int hovered = 0, int activated = 0);
		template<typename Component> void DrawSelectionComponent(const char* clabel, void(*show)(const Ref<Scene>&,const char*, Component*))
		{
			std::string label(clabel);
			if (s_Selection.Has<Component>() == 1) {
				if (ImGui::TreeNodeEx((const void *)(typeid(Component).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, label.c_str())) {
					show(p_Scene, ("##" + label).c_str(), s_Selection.Get<Component>());
					ImGui::Separator();
					ImGui::TreePop();
				}
			}
		}
	private:
		Ref<Scene> p_Scene;
	};
}
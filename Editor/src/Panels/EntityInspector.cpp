#include "EntityInspector.h"

namespace Kross {
	EntityInspector::EntityInspector(const char *name, Ref<Scene> scene)
		: p_Scene(scene)
	{
		SetName(name);
		KROSS_INFO("Entity Inspector Panel '{0}' Constructed", name);
	}
	void EntityInspector::Show(double ts)
	{
		if (!Manager().s_bEntityInspector) return;
		
		if (ImGui::Begin(GetName(), &Manager().s_bEntityInspector)) {
			ImGui::End();
		}
	}
}
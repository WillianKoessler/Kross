#include "EntityInspector.h"

namespace Kross {
	EntityInspector::EntityInspector(const Ref<Scene> &scene)
		: m_Context(scene)
	{
		m_strName = "Scene Inspector";
		m_Flags |= ImGuiWindowFlags_AlwaysAutoResize;
		if (!scene) KROSS_WARN("Scene supplied is nullptr");
		KROSS_INFO("Entity Inspector Panel '{0}' Constructed", m_strName);
	}
	void EntityInspector::Show(double ts)
	{
		if (!Manager().s_bEntityInspector) return;

		if (ImGui::Begin(m_strName, &Manager().s_bEntityInspector)) {
			if (m_Context) {
				auto all = m_Context->GetAllEntities();
				for (Entity &entity : all) {
					ImGui::Text("%s", entity.GetComponent<TagComponent>()->tag);
				}
			//	m_Context->m_Registry.each([&](auto e) {
			//		Entity entity{ (uint32_t)e, m_Context.get() };
			//		ImGui::Text("%s", entity.GetComponent<TagComponent>().tag);
			//	});
			}
			ImGui::End();
		}
	}
	void EntityInspector::DrawEntityNode(Entity &entity)
	{
		auto tc = entity.GetComponent<TagComponent>();
		if (tc)
		{
			ImGui::Text("%s", tc->tag);
		}
	}
}
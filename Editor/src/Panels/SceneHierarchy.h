#pragma once

#include "Panel.h"
#include "EntityProperties.h"

namespace Kross {
	class SceneHierarchy : public Panel
	{
	public:
		SceneHierarchy(const Ref<Scene>& scene);
		virtual ~SceneHierarchy() override;
		void SetContext(const Ref<Scene> &scene);
		virtual void Show(double ts) override;
	private:
		void DrawEntityNode(Entity &entity);
	private:
		Ref<Scene> p_Scene;
		Entity m_Clipboard;
	};
}

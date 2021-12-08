#pragma once

#include "Panel.h"

namespace Kross {
	class EntityInspector : public Panel
	{
		Ref<Scene> m_Context;
	public:
		EntityInspector(const Ref<Scene>& scene);
		virtual void Show(double ts) override;
	private:
		void DrawEntityNode(Entity &entity);
	};
}

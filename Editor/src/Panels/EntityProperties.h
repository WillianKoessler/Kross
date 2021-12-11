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
		Ref<Scene> p_Scene;
	};
}
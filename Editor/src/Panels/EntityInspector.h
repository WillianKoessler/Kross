#pragma once

#include "Panel.h"

namespace Kross {
	class EntityInspector : public Panel
	{
		Ref<Scene> p_Scene;
	public:
		EntityInspector(const char *name, Ref<Scene> scene);
		virtual void Show(double ts) override;
	};
}

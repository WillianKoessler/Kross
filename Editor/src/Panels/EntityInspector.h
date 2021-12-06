#pragma once

#include "Panel.h"

namespace Kross {
	class EntityInspector : public Panel
	{
		Ref<Scene> pScene;
	public:
		EntityInspector(const char *name);
		virtual void Show(double ts) override;
	};
}

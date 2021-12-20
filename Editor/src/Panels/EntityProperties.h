#pragma once

#include "Kross/Core/Resource.h"

namespace Kross {
	class EntityProperties : public Resource
	{
	public:
		EntityProperties(Scene &scene);
		void Show();
		void SetContext(Scene &scene) { p_Scene = &scene; };
	private:
		void DrawEntity(Entity &entity);
	private:
		Scene *p_Scene;
		uint32_t m_Flags;
	};
}

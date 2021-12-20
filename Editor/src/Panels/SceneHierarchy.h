#pragma once

#include "Kross/Core/Resource.h"

namespace Kross {
	class SceneHierarchy : public Resource
	{
	public:
		SceneHierarchy(Scene &scene);
		void Show();
		void SetContext(Scene &scene) { p_Scene = &scene; }
	private:
		void DrawEntityNode(Entity &entity);
	private:
		Scene *p_Scene;
		Entity m_Clipboard;
		uint32_t m_Flags;
	};
}

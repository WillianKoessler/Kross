#pragma once

#include "entt.hpp"

namespace Kross {
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const char* name);

		void OnUpdate(double ts);
	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}

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
		void SetPrimaryCamera(Entity& camera);

		void OnUpdate(double ts);
	private:
		entt::registry m_Registry;
		entt::entity m_PrimaryCamera = entt::null;
		friend class Entity;
	};
}

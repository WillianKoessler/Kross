#pragma once

#include "entt.hpp"

namespace Kross {
	class Scene
	{
	public:
		Scene();
		~Scene();

	private:
		entt::registry m_Registry;
	};
}

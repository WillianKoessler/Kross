#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace Kross {
	class Entity
	{
	public:
		Entity() = default;
		Entity::Entity(uint32_t id, Scene* scene) : m_ID((entt::entity)id), m_Scene(scene) {}
		Entity(const Entity& other) = default;

		template<typename T> bool HasComponent() { return m_Scene ? (m_Scene->m_Registry.any_of<T>(m_ID)) : false; }
		template<typename T> T& GetComponent()
		{
			if (!HasComponent<T>())
			{
				KROSS_CORE_WARN("[{0}] Entity does not have specified component", __FUNCTION__);
			}
			if (m_Scene)
				return m_Scene->m_Registry.get<T>(m_ID);
			else
			{
				KROSS_CORE_ERROR("[{0}] Invalid Scene pointer. (No valid scene was found)", __FUNCTION__);
				return T();
			}
		}

		template<typename T, typename...Args> T& AddComponent(Args&&...args)
		{
			if (m_Scene)
				return m_Scene->m_Registry.emplace<T>(m_ID, std::forward<Args>(args)...);
			else
			{
				KROSS_CORE_ERROR("[{0}] Invalid Scene pointer. (No valid scene was found)", __FUNCTION__);
				return T();
			}
		}

		template<typename T>
		void RemoveComponent()
		{
			if (m_Scene)
				return m_Scene->m_Registry.remove<T>(m_ID);
			else
				KROSS_CORE_ERROR("[{0}] Invalid Scene pointer. (No valid scene was found)", __FUNCTION__);
		}

		operator bool() const { return m_ID != entt::null; }

	private:
		entt::entity m_ID = entt::null;
		Scene* m_Scene = nullptr;
	};
}
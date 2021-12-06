#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace Kross {
	class Entity
	{
		using ecsEntity = entt::entity;
	public:
		Entity() = default;
		Entity(uint32_t id, Scene* scene) : m_ID((ecsEntity)id), p_Scene(scene) {}
		Entity(const Entity& other) = default;

		template<typename T> bool HasComponent() { return p_Scene ? (p_Scene->m_Registry.any_of<T>(m_ID)) : false; }
		template<typename T> T& GetComponent()
		{
			if (!HasComponent<T>())
			{
				KROSS_CORE_WARN("Entity does not have specified component");
			}
			if (p_Scene)
				return p_Scene->m_Registry.get<T>(m_ID);
			else
			{
				KROSS_CORE_ERROR("Invalid Scene pointer. (No valid scene was found)");
				return T();
			}
		}

		template<typename T, typename...Args> T& AddComponent(Args&&...args)
		{
			if (p_Scene)
				return p_Scene->m_Registry.emplace<T>(m_ID, std::forward<Args>(args)...);
			else
			{
				KROSS_CORE_ERROR("Invalid Scene pointer. (No valid scene was found)");
				return T();
			}
		}

		template<typename T>
		void RemoveComponent()
		{
			if (p_Scene)
				return p_Scene->m_Registry.remove<T>(m_ID);
			else
				KROSS_CORE_ERROR("Invalid Scene pointer. (No valid scene was found)");
		}

		operator bool() const { return m_ID != entt::null; }

	private:
		entt::entity m_ID = entt::null;
		Scene* p_Scene = nullptr;
	};
}
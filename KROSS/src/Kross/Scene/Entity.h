#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace Kross {
	class Entity
	{
	public:
		Entity() = default;
		Entity::Entity(entt::entity id, Scene* scene) : m_ID(id), p_Scene(scene) {}
		Entity(const Entity& other) = default;

		template<typename T>
		bool HasComponent() const
		{
			return true;
//			if (!p_Scene) return false;
//			return p_Scene->m_Registry.all_of<T>(m_ID);
		}
		template<typename T>
		T* GetComponent()
		{
			//if (!HasComponent<T>())
			//{
			//	KROSS_CORE_WARN("[{0}] Entity does not have specified component", __FUNCTION__);
			//}
			if (p_Scene)
				return &p_Scene->m_Registry.get<T>(m_ID);
			else
			{
				KROSS_CORE_ERROR("[{0}] Invalid Scene pointer. (No valid scene was found)", __FUNCTION__);
				return nullptr;
			}
		}

		template<typename T, typename...Args>
		T* AddComponent(Args&&...args)
		{
			if (p_Scene)
			{
				KROSS_CORE_TRACE("[{0}] Component added to Entity (ID = {1})", __FUNCTION__, (uint32_t)m_ID);
				return &p_Scene->m_Registry.emplace<T>(m_ID, std::forward<Args>(args)...);
			}
			else
			{
				KROSS_CORE_ERROR("[{0}] Invalid Scene pointer. (No valid scene was found)", __FUNCTION__);
				return nullptr;
			}
		}

		template<typename T>
		void RemoveComponent()
		{
			if (p_Scene)
			{
				KROSS_CORE_TRACE("[{0}] Component removed from Entity (ID = {1})", __FUNCTION__, (uint32_t)m_ID);
				return p_Scene->m_Registry.remove<T>(m_ID);
			}
			else
				KROSS_CORE_ERROR("[{0}] Invalid Scene pointer. (No valid scene was found)", __FUNCTION__);
		}

		entt::entity getID() const { return m_ID; }
		inline bool isValid() { return m_ID != entt::null; }
		operator bool() const { return m_ID != entt::null; }
		bool operator ==(entt::entity other) { return m_ID == other; }

	private:
		entt::entity m_ID = entt::null;
		Scene* p_Scene = nullptr;
	};
}

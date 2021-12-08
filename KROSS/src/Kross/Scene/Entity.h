#pragma once

#include "Scene.h"
#include "EmptyComponent.h"

namespace Kross {
	class Entity
	{
		friend class Scene;
	public:
		Entity() = default;
		Entity(uint32_t id, Scene *scene) : m_ID((entt::entity)id), p_Scene(scene) {}
		Entity(const Entity &other) = default;

		template<typename T> int HasComponent() { return p_Scene ? (int)p_Scene->m_Registry.any_of<T>(m_ID) : -1; }
		template<typename T> T *GetComponent()
		{
			if (HasComponent<T>() != -1) { KROSS_WARN("Entity does not have specified component. ID={0}, ({1})",(uint32_t)m_ID, typeid(T).name()); return nullptr; }
			if (p_Scene) return &p_Scene->m_Registry.get<T>(m_ID);
			else KROSS_ERROR("Invalid Scene pointer. (No valid Scene was found)"); return nullptr;
		}

		template<typename T, typename...Args> T *AddComponent(Args&&...args)
		{
			if (HasComponent<T>()) KROSS_WARN("Entity already have specified component. ID={0}, ({1})", (uint32_t)m_ID, typeid(T).name());
			if (p_Scene) return &p_Scene->m_Registry.emplace<T>(m_ID, std::forward<Args>(args)...);
			else KROSS_ERROR("Invalid Scene pointer. (No valid Scene was found)"); return nullptr;
		}

		template<typename T>
		void RemoveComponent()
		{
			if (p_Scene) p_Scene->m_Registry.remove<T>(m_ID);
			else KROSS_ERROR("Invalid Scene pointer. (No valid Scene was found)");
		}

		operator bool() const { return m_ID != entt::null; }
		operator uint32_t() const { return (uint32_t)m_ID; }
		bool operator==(const Entity &other) const { return p_Scene == other.p_Scene && m_ID == other.m_ID; }
		bool operator!=(const Entity &other) const { return p_Scene != other.p_Scene && m_ID != other.m_ID; }
	private:
		entt::entity m_ID = entt::null;
		Scene *p_Scene = nullptr;
	};
}
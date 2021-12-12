#pragma once

#include "Scene.h"
#include "EmptyComponent.h"

namespace Kross {
	class Entity
	{
		friend class Scene;
	public:
		Entity() = default;
		Entity(uint32_t id, const Scene *scene) : m_ID((entt::entity)id), p_Scene(const_cast<Scene *>(scene)) {}
		Entity(const Entity &other) = default;

		//template<typename Component> int Has() const
		//{
		//	if (!p_Scene) return -1;
		//	if (m_ID == entt::null) return -2;
		//	return (int)p_Scene->m_Registry.any_of<Component>(m_ID);
		//}
		template<typename...Components> int Has() const
		{
			if (!p_Scene) return -1;
			if (m_ID == entt::null) return -2;
			return (int)p_Scene->m_Registry.all_of<Components...>(m_ID);
		}
		template<typename T> T *Get()
		{
			if (p_Scene) {
				if (Has<T>() == 1) return &p_Scene->m_Registry.get<T>(m_ID);
				else KROSS_WARN("Entity (ID = '{0}') does not have specified component '{1}'.", (uint32_t)m_ID, typeid(T).name());
			} else KROSS_ERROR("Invalid Scene pointer. (No valid Scene was found) ");
			return nullptr;
		}

		template<typename T, typename...Args> T *Add(Args&&...args)
		{
			if (p_Scene) {
				if (Has<T>() == 1) KROSS_WARN("Entity (ID = '{0}') already have specified component '{1}'.", (uint32_t)m_ID, typeid(T).name());
				else return &p_Scene->m_Registry.emplace<T>(m_ID, std::forward<Args>(args)...);
			} else KROSS_ERROR("Invalid Scene pointer. (No valid Scene was found) ");
			return nullptr;
		}

		template<typename T>
		void Remove()
		{
			if (p_Scene) p_Scene->m_Registry.remove<T>(m_ID);
			else KROSS_ERROR("Invalid Scene pointer. (No valid Scene was found)");
		}


		const Scene *GetScene() const { return p_Scene; }
		operator bool() const { return m_ID != entt::null; }
		operator uint32_t() const { return (uint32_t)m_ID; }
		operator entt::entity() const { return m_ID; }
		operator void *() const { return (void *)(uintptr_t)m_ID; }
		bool operator==(const Entity &other) const { return p_Scene == other.p_Scene && m_ID == other.m_ID; }
		bool operator!=(const Entity &other) const { return p_Scene != other.p_Scene && m_ID != other.m_ID; }
	private:
		entt::entity m_ID = entt::null;
		Scene *p_Scene = nullptr;
	};
}
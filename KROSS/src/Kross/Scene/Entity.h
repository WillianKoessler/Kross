#pragma once

#include "Kross/Core/Core.h"
#include "Scene.h"
#include "EmptyComponent.h"

namespace Kross {
	class Entity
	{
		friend class Scene;
		template<typename Component>
		Component *GetImpl()
		{
			KROSS_NOT_IMPLEMENTED;
		}
	public:
		Entity() = default;
		Entity(entt::entity id, const Scene *scene)
			: m_ID(id), p_Scene(const_cast<Scene *>(scene))
		{
			KROSS_ASSERT(p_Scene != nullptr, "Invalid Scene pointer. (No valid Scene was found)");
		}
		Entity(const Entity &other) = default;

		template<typename...Components>
		int Has() const
		{
			KROSS_ASSERT(p_Scene != nullptr, "Invalid Scene pointer. (No valid Scene was found)");
			if (m_ID == entt::null) return -1;
			if (!p_Scene->m_Registry.valid(m_ID)) return -2;
			return (int)p_Scene->m_Registry.all_of<Components...>(m_ID);
		}

		template<typename...Component>
		auto Get()
		{
			static bool validation = true;
			static constexpr char *msg = "Entity (ID = '{0}') does not have specified component.";
			//if (Validate(&validation, Has<Component...>(), msg, (uint32_t)m_ID))
				return p_Scene->m_Registry.try_get<Component...>(m_ID);
			//return nullptr;
		}

		template<typename Component, typename...Args>
		Component *Add(Args&&...args)
		{
			static bool validation = true;
			const char *msg = "Entity (ID = '{0}') already have specified component '{1}'.";
			if (!Validate(&validation, !Has<Component>(), msg, (uint32_t)m_ID, typeid(Component).name()))
				return nullptr;
			auto cmp = &p_Scene->m_Registry.emplace<Component>(m_ID, std::forward<Args>(args)...);
			p_Scene->OnComponentAdded(*this, typeid(Component).hash_code());
			return cmp;
		}

		template<typename Component>
		void Remove()
		{
			if (Has<Component>() == 1)
				p_Scene->m_Registry.remove<Component>(m_ID);
		}


		const Scene *GetScene() const { return p_Scene; }
		operator bool() const { return m_ID != entt::null; }
		operator uint32_t() const { return (uint32_t)m_ID; }
		operator entt::entity() const { return m_ID; }
		bool operator==(const Entity &other) const { return p_Scene == other.p_Scene && m_ID == other.m_ID; }
		bool operator!=(const Entity &other) const { return p_Scene != other.p_Scene && m_ID != other.m_ID; }
	private:
		entt::entity m_ID = entt::null;
		Scene *p_Scene = nullptr;
	};
}

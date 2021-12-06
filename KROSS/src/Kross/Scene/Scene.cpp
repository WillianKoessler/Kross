#include "Kross_pch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Kross/Renderer/Renderer2D.h"

namespace Kross {
	Scene::Scene()
	{
		m_Registry = entt::registry();
		KROSS_CORE_INFO("Scene Constructed");
	}
	Scene::~Scene()
	{
		KROSS_CORE_INFO("Scene Destructed");
	}

	Entity Scene::CreateEntity(const char* name)
	{
		Entity entity{ (uint32_t)m_Registry.create(), this};
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name);
		KROSS_CORE_TRACE("Entity '{0}' Created", name);
		return entity;
	}

	void Scene::OnUpdate(double ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
		for (auto entity : group)
		{
			auto& [tranform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
			Renderer2D::BatchQuad(tranform, sprite.tint);
		}
	}
}
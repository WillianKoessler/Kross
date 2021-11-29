#include "Kross_pch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Kross/Renderer/Renderer2D.h"

namespace Kross {
	Scene::Scene()
	{
	}
	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const char* name)
	{
		Entity entity{ (uint32_t)m_Registry.create(), this};
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name);
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
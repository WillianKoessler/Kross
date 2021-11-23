#include "Kross_pch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Kross/Renderer/Renderer2D.h"

namespace Kross {
	Scene::Scene()
	{
		KROSS_CORE_INFO("[{0}] Scene Created", __FUNCTION__);
	}
	Scene::~Scene()
	{
		KROSS_CORE_INFO("[{0}] Scene Destructed", __FUNCTION__);
	}

	Entity Scene::CreateEntity(const char* name)
	{
		Entity entity{ m_Registry.create(), this};
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name);
		KROSS_CORE_INFO("[{0}] new Entity created. Tag={1}", __FUNCTION__, name);
		return entity;
	}
	void Scene::SetPrimaryCamera(Entity& camera)
	{
		//if (m_Registry.valid(camera.getID()))
		{
			m_PrimaryCamera = camera.getID();
			KROSS_CORE_TRACE("[{0}] Primary Camera Set.");
		}
		//else KROSS_CORE_ERROR("[{0}] Invalid camera was set to be the primary camera.", __FUNCTION__);
	}
	void Scene::OnUpdate(double ts)
	{
		//Render
		Entity Camera{ m_PrimaryCamera, this };
		if (Camera)
		{
			Renderer2D::Begin(Camera.GetComponent<CameraComponent>()->camera);
			Renderer2D::BatchBegin();
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group)
			{
				auto& [tranform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
				Renderer2D::BatchQuad(tranform, sprite.tint);
			}
			Renderer2D::BatchEnd();
			Renderer2D::End();
		}
	}
}
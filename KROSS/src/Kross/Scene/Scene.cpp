#include "Kross_pch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "Kross/Renderer/Renderer2D.h"
#include "Kross/Renderer/Cameras/Cameras/Orthographic.h"
#include "Kross/Renderer/Cameras/Cameras/Perspective.h"

namespace Kross {
	Scene::Scene()
	{
		m_Registry = entt::registry();
		KROSS_CORE_INFO("Scene Constructed");
	}
	Scene::~Scene()
	{
		m_Registry.view<NativeScriptComponent>().each(
			[](auto entity, auto &cmp) { if (cmp.m_Instance) { cmp.m_Instance->OnDestroy(); cmp.Destroy(&cmp); }}
		);
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

	Entity Scene::GetEntity(const char *name)
	{
		auto view = m_Registry.view<TagComponent>();
		std::vector<entt::entity> pool;
		for (auto e : view) {
			auto& [Tag] = view.get(e);
			if (strcmp(Tag.tag, name) == 0) pool.push_back(e);
		}
		if (pool.size() > 1) { KROSS_CORE_ERROR("More than one entity shares the same Tagname"); return Entity(); }
		if (pool.empty()) { KROSS_CORE_ERROR("There is no entity with that Tagname"); return Entity(); }
		return Entity((uint32_t)pool[0], this);
	}

	void Scene::OnUpdateEditor(double ts, const Camera::Editor &camera)
	{
		Renderer2D::Begin(camera);
		{
			// Render Sprites
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group)
			{
				auto &[tranform, sprite] = group.get(entity);
				Renderer2D::BatchQuad(tranform, sprite.tint);
			}
		}
		Renderer2D::End();
	}

	void Scene::OnUpdateRuntime(double ts)
	{
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& cmp) {
			if (!cmp.m_Instance) {
				cmp.m_Instance = cmp.Instantiate();
				cmp.m_Instance->m_Entity = Entity( (uint32_t)entity, this );
				cmp.m_Instance->OnCreate();
			}
			cmp.m_Instance->OnUpdate(ts);
			});

		if (primaryCamera != entt::null) {
			Entity camera((uint32_t)primaryCamera, this);
			Renderer2D::Begin(camera.GetComponent<CameraComponent>(), camera.GetComponent<TransformComponent>());
			{
				// Render Sprites
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
				for (auto entity : group)
				{
					auto &[tranform, sprite] = group.get(entity);
					Renderer2D::BatchQuad(tranform, sprite.tint);
				}
			}
			Renderer2D::End();
		}
	}

	void Scene::OnViewportResize(const glm::uvec2 &size)
	{
		m_ViewportSize = size;

		auto view = m_Registry.view<CameraComponent>();
		view.each([&size](CameraComponent &cmpt) { if (!cmpt.fixedAspectRatio) { cmpt.camera.SetViewportSize(size); }});
	}

	void Scene::SetPrimaryCamera(Entity entity)
	{
		if (entity.p_Scene != this) { KROSS_CORE_WARN("Trying to set a Primary Camera with entity from another Scene."); return; }
		if (!entity.HasComponent<CameraComponent>()) { KROSS_CORE_WARN("Entity does not have a Camera Component to be setted as Scene's Primary Camera."); return; }
		if (primaryCamera != entt::null) KROSS_CORE_WARN("Overriding previous Camera.");
		if (m_Registry.view<CameraComponent>().contains(entity.m_ID)) primaryCamera = entity.m_ID;
		KROSS_CORE_TRACE("Scene Primary Camera has been set.");
	}
}
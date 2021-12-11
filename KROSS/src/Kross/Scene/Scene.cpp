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
		KROSS_INFO("Scene Constructed");
	}
	Scene::~Scene()
	{
		m_Registry.view<NativeScriptComponent>().each(
			[](auto entity, auto &cmp) { if (cmp.m_Instance) { cmp.m_Instance->OnDestroy(); cmp.Destroy(&cmp); }}
		);
		KROSS_INFO("Scene Destructed");
	}

	Entity Scene::CreateEntity(const char *name)
	{
		static std::vector<std::string> usedNames;
		for (auto &stored : usedNames) { std::string now(name); if (stored == now) { KROSS_WARN("Entity Tagname is already in use."); return Entity(); } }
		Entity entity{ (uint32_t)m_Registry.create(), this };
		entity.Add<TransformComponent>();
		entity.Add<TagComponent>(name);
		KROSS_TRACE("Entity '{0}' Created", name);
		usedNames.push_back(std::string(name));
		return entity;
	}

	Entity Scene::GetEntity(const char *name) const
	{
		auto view = m_Registry.view<TagComponent>();
		std::vector<entt::entity> pool;
		for (auto e : view) {
			auto [Tag] = view.get(e);
			if (strcmp(Tag.tag, name) == 0) pool.push_back(e);
		}
		if (pool.size() > 1) { KROSS_ERROR("More than one entity shares the same Tagname"); return Entity(); }
		if (pool.empty()) { KROSS_ERROR("There is no entity with that Tagname"); return Entity(); }
		return Entity((uint32_t)pool.front(), this);
	}
#ifdef KROSS_DLL
	Scene::Entities Scene::GetAllEntities()
	{
		static std::vector<Entity> pool;
		pool = std::vector<Entity>();
		m_Registry.each([&](auto &id) { pool.emplace_back((uint32_t)id, this); });
		return Entities{ (uintptr_t)pool.data(), pool.size(), sizeof(Entity) };
	}
#else
	std::vector<Entity> Scene::GetAllEntities()
	{
		std::vector<Entity> pool;
		m_Registry.each([&](auto &id) {pool.emplace_back((uint32_t)id, this); });
		return pool;
	}
#endif
	void Scene::SetPrimaryCamera(const Entity &entity)
	{
		if (entity.p_Scene != this) { KROSS_WARN("Trying to set a Primary Camera with entity from another Scene."); return; }
		if (!entity.Has<CameraComponent>()) { KROSS_WARN("Entity does not have a Camera Component to be setted as Scene's Primary Camera."); return; }
		if (!entity.Has<TransformComponent>()) { KROSS_WARN("Entity does not have a Transform Component to be setted as Scene's Primary Camera."); return; }
		if (!entity.Has<TagComponent>()) { KROSS_WARN("Entity does not have a Tag Component to be setted as Scene's Primary Camera."); return; }
		if (m_Registry.view<CameraComponent>().contains(entity.m_ID)) m_PrimaryCameraID = entity.m_ID;
		else KROSS_WARN("Scene does not contain entity supplied to be Primary Camera.");
	}
	Entity Scene::GetCurrentCamera() const
	{
		return Entity{ (uint32_t)m_PrimaryCameraID, this };
	}
	void Scene::OnUpdateEditor(double ts, const Camera::Editor &camera)
	{
		Renderer2D::ResetStats();

		Renderer2D::Begin(camera);
		{
			// Render Sprites
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group)
			{
				auto [tranform, sprite] = group.get(entity);
				Renderer2D::BatchQuad(tranform, sprite.tint);
			}
		}
		Renderer2D::End();
	}

	void Scene::OnUpdateRuntime(double ts)
	{
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto &cmp) {
			if (!cmp.m_Instance) {
				cmp.m_Instance = cmp.Instantiate();
				cmp.m_Instance->m_Entity = Entity((uint32_t)entity, this);
				cmp.m_Instance->OnCreate();
			}
			cmp.m_Instance->OnUpdate(ts);
			});

		if (m_PrimaryCameraID != entt::null) {
			Entity primaryCamera((uint32_t)m_PrimaryCameraID, this);
			auto camera = primaryCamera.Get<CameraComponent>();
			auto tranform = primaryCamera.Get<TransformComponent>();
			if (!camera) KROSS_WARN("Primary Camera does not have CameraComponent");
			if (!tranform) KROSS_WARN("Primary Camera does not have TransformComponent");
			if (!camera || !tranform) return;
			Renderer2D::Begin(*camera, *tranform);
			{
				// Render Sprites
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);

				for (auto entity : group)
				{
					auto [tranform, sprite] = group.get(entity);
					Renderer2D::BatchQuad(tranform, sprite.tint);
				}
			}
			Renderer2D::End();
		}
	}

	void Scene::OnViewportResize(const glm::vec2 &size)
	{
		m_ViewportSize = size;
		auto view = m_Registry.view<CameraComponent>();
		view.each([&size](CameraComponent &cmpt) { if (!cmpt.fixedAspectRatio) { cmpt.camera.SetViewportSize(size); }});
	}
}

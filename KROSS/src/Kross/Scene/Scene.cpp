#include <Kross_pch.h>
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "SceneSerializer.h"
#include "Kross/Renderer/Renderer2D.h"
#include "Kross/Renderer/RendererCommands.h"
#include "Kross/Renderer/Cameras/Cameras/Orthographic.h"
#include "Kross/Renderer/Cameras/Cameras/Perspective.h"
#include "Kross/Core/Input.h"

namespace Kross {
	Scene::Scene()
	{
		SetName("Unnamed");
		KROSS_INFO("Scene Constructed");
	}
	Scene::Scene(const char *name)
	{
		SetName(name);
		KROSS_INFO("Scene '{0}' Constructed", m_strName);
	}
	Scene &Scene::operator=(const Scene &other)
	{
		SetName(other.m_strName);
		m_namePool = other.m_namePool;
		m_Registry = entt::registry();
		m_PrimaryCameraID = entt::null;
		m_Selection = entt::null;
		m_ViewportSize = other.m_ViewportSize;
		file = other.file;

		KROSS_INFO("Scene '{0}' Assigned", m_strName);
		return *this;
	}
	Scene::~Scene()
	{
		Clear();
		if (m_strName) {
			KROSS_INFO("Scene '{0}' Destructed", m_strName);
			delete m_strName;
		} else {
			KROSS_INFO("Unnamed Scene Destructed");
		}
	}
	
	void Scene::SetName(const char *name)
	{
		if (name != nullptr) {
			if (m_strName != nullptr) {
				if (strcmp(name, m_strName) == 0) return;
				delete m_strName;
			}
			size_t size = strlen(name) + 1;
			m_strName = new char[size];
			memset(m_strName, 0, size);
			strcpy_s(m_strName, size, name);
		} else {
			if (m_strName) delete m_strName;
		}
	}

	void Scene::SaveScene()
	{
		SceneSerializer ss(this);
		if (file) ss.Serialize(file);
		else {
			file = FileDialog::SaveFile("Kross Scene (.kross)\0*.kross\0\0");
			if (file) ss.Serialize(file);
			else KROSS_WARN("Scene was not saved");
		}
	}
	void Scene::SaveScene(const File &to_file)
	{
		SceneSerializer ss(this);
		if (!to_file) return;
		file = to_file;
		SaveScene();
	}
	void Scene::LoadScene()
	{
		SceneSerializer ss(this);
		file = FileDialog::OpenFile("Kross Scene (.kross)\0*.kross\0\0");
		if (file) ss.Deserialize(file);
		else KROSS_WARN("Scene was not loaded");
	}
	void Scene::Select(Entity &e)
	{
		if (e) m_Selection = e;
	}
	Entity Scene::Selected() const
	{
		return Entity{ m_Selection, this };
	}
	void Scene::ClearSelection()
	{
		m_Selection = entt::null;
	}
	Entity Scene::CreateEntity(const char *name)
	{
		Entity entity{ m_Registry.create(), this };
		const char *tag = entity.Add<TagComponent>(name, &m_namePool)->Get();
		return entity;
	}

	template<typename Component>
	static bool copyTo(Entity &to, Entity &from)
	{
		if (from.Has<Component>() == 1) {
			to.Add<Component>(*from.Get<Component>());
			return true;
		}
		return false;
	}
	Entity Scene::CreateEntity(Entity e)
	{
		Entity entity{ m_Registry.create(), this };
		copyTo<TagComponent>(entity, e);
		copyTo<TransformComponent>(entity, e);
		copyTo<SpriteComponent>(entity, e);
		copyTo<CameraComponent>(entity, e);
		copyTo<NativeScriptComponent>(entity, e);
		return entity;
	}

	Entity Scene::GetEntity(const char *name) const
	{
		auto view = m_Registry.view<TagComponent>();
		std::vector<entt::entity> pool;
		for (auto e : view)
			if (!strcmp(view.get(e)._Myfirst._Val, name)) pool.push_back(e);
		if (pool.size() > 1) KROSS_ERROR("More than one entity shares the same Tagname");
		if (pool.empty()) { KROSS_ERROR("There is no entity with that Tagname"); return Entity(); }
		return Entity{ pool.front(), this };
	}
	void Scene::DestroyEntity(Entity e)
	{
		if (e) {
			ClearSelection();
			if (e.Has<TagComponent>())
				e.Get<TagComponent>()->Set(nullptr);
			m_Registry.destroy(e);
		}
	}
	void Scene::Clear()
	{
		ClearSelection();
		m_Registry.view<NativeScriptComponent>().each(
			[](auto entity, auto &cmp) { if (cmp.m_Instance) { cmp.m_Instance->OnDestroy(); cmp.Destroy(&cmp); }}
		);
		m_Registry.view<TagComponent>().each(
			[this](auto entity, auto &cmp) { cmp.Set(nullptr); m_Registry.destroy(entity);}
		);

		m_Registry.clear();
		//m_Registry = entt::registry();
	}

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
		return Entity{ m_PrimaryCameraID, this };
	}
	void Scene::OnUpdateEditor(double ts, const Camera::Editor &camera)
	{
		RenderCommand::Clear();
		Renderer2D::ResetStats();
		Renderer2D::Begin(camera);
		auto view = m_Registry.view<TransformComponent, SpriteComponent>();
		for (auto [entity, transform, sprite] : view.each())
			Renderer2D::BatchQuad(transform, sprite.tint, sprite.texture);
		Renderer2D::End();
	}
	void Scene::OnUpdateRuntime(double ts)
	{
		Entity primaryCamera{ m_PrimaryCameraID, this };
		int valid = primaryCamera.Has<TransformComponent, CameraComponent>();

		static bool validated = false;
		if (!Validate(&validated, valid == 1, "PrimaryCamera is not valid"))
			return;

		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto &cmp) {
			if (!cmp.m_Instance) {
				cmp.m_Instance = cmp.Instantiate();
				cmp.m_Instance->m_Entity = Entity{ entity, this };
				cmp.m_Instance->OnCreate();
			}
			cmp.m_Instance->OnUpdate(ts);
			});

		auto camera = primaryCamera.Get<CameraComponent>();
		auto transform = primaryCamera.Get<TransformComponent>();
		Renderer2D::ResetStats();
		RenderCommand::Clear();
		Renderer2D::Begin(camera->camera, *transform);
		auto group = m_Registry.group<TransformComponent, SpriteComponent>();

		for (auto [entity, transform, sprite] : group.each()) {
			Renderer2D::BatchQuad(transform, sprite.tint, sprite.texture);
		}
		Renderer2D::End();
	}

	void Scene::OnViewportResize(const glm::vec2 &size)
	{
		m_ViewportSize = size;
		auto view = m_Registry.view<CameraComponent>();
		view.each([&size](CameraComponent &cmpt) { if (!cmpt.fixedAspectRatio) { cmpt.camera.SetViewportSize(size); }});
	}
	void Scene::OnComponentAdded(Entity e, uint64_t componentID)
	{
		if (componentID == typeid(TagComponent).hash_code()) {
		} else if (componentID == typeid(TransformComponent).hash_code()) {
		} else if (componentID == typeid(SpriteComponent).hash_code()) {
		} else if (componentID == typeid(CameraComponent).hash_code()) {
			e.Get<CameraComponent>()->camera.SetViewportSize(m_ViewportSize);
		} else if (componentID == typeid(NativeScriptComponent).hash_code()) {
		}
	}
}

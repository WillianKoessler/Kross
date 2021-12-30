#pragma once

#include "Kross/Core/Core.h"
#include "entt.hpp"
#include "Kross/Renderer/Cameras/Camera.h"
#include "Kross/Renderer/Cameras/EditorCamera.h"
#include "Kross/Util/PlatformUtils.h"

namespace Kross {
	class Entity;
	class SceneSerializer;
	class Scene
	{
	public:
		Scene();
		Scene(const char *name);
		Scene &operator=(const Scene &other);
		~Scene();


		const char *GetName() const { return m_strName; }
		void SetName(const char *name);

		void SaveScene();
		void SaveScene(const File &file);
		void LoadScene();
		void LoadScene(const File &file);

		void Select(Entity &);
		Entity Selected() const;
		void ClearSelection();

		Entity CreateEntity(const char *Tag);
		Entity CreateEntity(Entity e);
		Entity GetEntity(const char *Tag) const;
		void DestroyEntity(Entity e);
		void Clear();

		std::vector<Entity> GetAllEntities() // TODO: create Kross's Vector class
		{
			std::vector<Entity> pool;
			m_Registry.each([&pool, this](auto &id) {pool.emplace_back(id, this); });
			return pool;
		}

		void SetPrimaryCamera(const Entity &entity);
		Entity GetCurrentCamera() const;

		void OnUpdateEditor(double ts, const Camera::Editor &camera);
		void OnUpdateRuntime(double ts);
		void OnViewportResize(const glm::vec2 &size);

	private:
		void OnComponentAdded(Entity e, uint64_t componentID);

	private:
		std::vector<std::string> m_namePool; // TODO: create Kross's Vector and String classes
		entt::registry m_Registry;
		char *m_strName = nullptr;
		entt::entity m_PrimaryCameraID = entt::null, m_Selection = entt::null;
		glm::vec2 m_ViewportSize;
		File file;

		friend class Entity;
		friend class SceneSerializer;
	};
}

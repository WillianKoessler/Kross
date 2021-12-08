#pragma once

#include "entt.hpp"
#include "Kross/Renderer/Cameras/Camera.h"
#include "Kross/Renderer/Cameras/EditorCamera.h"

namespace Kross {
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const char *Tag);
		Entity GetEntity(const char *Tag);
		void SetPrimaryCamera(Entity entity);

		void OnUpdateEditor(double ts, const Camera::Editor& camera);
		void OnUpdateRuntime(double ts);
		void OnViewportResize(const glm::uvec2& size);
	private:
		entt::registry m_Registry;
		entt::entity primaryCamera = entt::null;
		glm::uvec2 m_ViewportSize;

		friend class Entity;
	};
}

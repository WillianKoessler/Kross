#pragma once

#include <glm/glm.hpp>
#include <stdlib.h>
#include <vector>
#include "Kross/Util/Constants.h"
#include "SceneCamera.h"
#include "Kross/Renderer/Cameras/Cameras/Orthographic.h"
#include "ScriptableEntity.h"
#include "EmptyComponent.h"


namespace Kross {
	struct TagComponent : public EmptyComponent
	{
		static const uint32_t limit = 128;
	private:
		char tag[limit];
	public:
		void Set(const char *newTag);
		const char *Get() const { return tag; }

		~TagComponent();
		TagComponent() { Set(nullptr); }
		TagComponent(const TagComponent &other) { Set(other); }
		TagComponent(const char *newTag) { Set(newTag); }

		operator const char*() const { return tag; }
	};

	struct TransformComponent : public EmptyComponent
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent &) = default;
		TransformComponent(const glm::vec3 &position)
			: Position(position) {}

		operator glm::mat4() const
		{
			return (
				glm::translate(const_mat4, Position) *
				(glm::rotate(const_mat4, glm::radians(Rotation.x), { 1.0f, 0.0f, 0.0f }) *
				glm::rotate(const_mat4, glm::radians(Rotation.y), { 0.0f, 1.0f, 0.0f }) *
				glm::rotate(const_mat4, glm::radians(Rotation.z), { 0.0f, 0.0f, 1.0f })) *
				glm::scale(const_mat4, Scale)
				);
		}
	};

	struct SpriteComponent : public EmptyComponent
	{
		glm::vec4 tint = glm::vec4(1.0f);
		//Ref<Texture::T2D> sprite = nullptr;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent &) = default;
		SpriteComponent(const glm::vec4 &tintColor)
			: tint(tintColor) {}
	};

	struct CameraComponent : public EmptyComponent
	{
		SceneCamera camera;
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent &) = default;
		CameraComponent(const SceneCamera &c)
			: camera(c) {}
	};

	struct NativeScriptComponent
	{
		ScriptableEntity *m_Instance = nullptr;

		ScriptableEntity *(*Instantiate)();
		void(*Destroy)(NativeScriptComponent *);

		template<typename T>
		void Bind()
		{
			Instantiate = []() { return static_cast<ScriptableEntity *>(new T); };
			Destroy = [](NativeScriptComponent *p) { delete p->m_Instance; p->m_Instance = nullptr; };
		}
	};
}
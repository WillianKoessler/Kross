#pragma once

#include <glm/glm.hpp>
#include <stdlib.h>
#include "SceneCamera.h"
#include "Kross/Renderer/Cameras/Cameras/Orthographic.h"
#include "ScriptableEntity.h"
#include "EmptyComponent.h"

namespace Kross {
	struct TransformComponent : public EmptyComponent
	{
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& mat)
			: Transform(mat) { }

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteComponent : public EmptyComponent
	{
		glm::vec4 tint = glm::vec4(1.0f);
		//Ref<Texture::T2D> sprite = nullptr;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec4& tintColor)
			: tint(tintColor) { }
	};

	struct TagComponent : public EmptyComponent
	{
		static const uint32_t limit = 128;
		char tag[limit];

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const char *newTag) { memset(tag, 0, sizeof(tag)); memcpy(tag, newTag, sizeof(tag)); }
	};

	struct RefCameraComponent : public EmptyComponent
	{
		Ref<Camera::Camera> camera = makeRef<Camera::Orthographic>("UnnamedRefCameraComponent", -16.0f, 16.0f, -9.0f, 9.0f);

		RefCameraComponent() = default;
		RefCameraComponent(const RefCameraComponent&) = default;
		RefCameraComponent(const Ref<Camera::Camera>& camera)
			: camera(camera) {}
	};

	struct CameraComponent : public EmptyComponent
	{
		SceneCamera camera;
		bool Active = false;
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent &) = default;
		CameraComponent(const SceneCamera &c)
			: camera(c) {}
	};

	struct NativeScriptComponent
	{
		ScriptableEntity *m_Instance = nullptr;

		ScriptableEntity*(*Instantiate)();
		void(*Destroy)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			Instantiate = []() { return static_cast<ScriptableEntity *>(new T); };
			Destroy = [](NativeScriptComponent *p) { delete p->m_Instance; p->m_Instance = nullptr; };
		}
	};
}
#pragma once

#include <glm/glm.hpp>
#include "Kross/Scene/ScriptableEntity.h"
#include "Kross/Core/Input.h"
#include "Kross/Scene/Components.h"

namespace Kross {
	class CameraController : public ScriptableEntity
	{
		glm::vec3 vel;
	public:
		void OnCreate() { vel = glm::vec3(0.0f); }
		void OnUpdate(double ts)
		{
			float speed = 8.0f * (float)ts;
			glm::vec3 acc = glm::vec3(0.0f);
			if (Input::IsKeyHeld(Key::Up)) acc.y += speed;
			if (Input::IsKeyHeld(Key::Down)) acc.y -= speed;
			if (Input::IsKeyHeld(Key::Right)) acc.x += speed;
			if (Input::IsKeyHeld(Key::Left)) acc.x -= speed;
			vel = (vel * 0.95f) + acc;
			Get<TransformComponent>().Position += vel;
		}
	};
}
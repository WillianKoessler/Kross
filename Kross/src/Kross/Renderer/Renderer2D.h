#pragma once

#include "Cameras/Camera.h"
#include "Cameras/EditorCamera.h"
#include "Kross/Scene/SceneCamera.h"
#include "Textures/Textures.h"
#include "Textures/Atlas.h"
#include "Shaders.h"

namespace Kross {
	struct QuadParams;
	class Renderer2D
	{
		static bool s_bSceneBegan;
		static bool s_bBatch;
		static bool s_bInitiated;
		
		static void BatchBegin();
		static void Flush();
		static void BatchEnd();
	public:
		struct Stats
		{
			uint32_t DrawCount = 0;
			uint32_t QuadCount = 0;
		};

		Renderer2D() = delete;
		Renderer2D(Renderer2D&& other) = delete;
		Renderer2D& operator =(Renderer2D&& other) = delete;

		static const Stats& getStats();
		static void ResetStats();

		static void Init();
		static void Shutdown();

		static void Begin(const Camera::Editor& camera);
		static void Begin(const SceneCamera& camera, const glm::mat4& transform);
		static void Begin(Ref<Camera::Camera>& camera);
		static void End();

		static void SwitchShader(const Ref<Shader>& shader);

		static void BatchQuad(const QuadParams& params);
		static void BatchQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture::T2D>& sprite = nullptr, int id = -1);

		static void DebugAxis();
	};

}

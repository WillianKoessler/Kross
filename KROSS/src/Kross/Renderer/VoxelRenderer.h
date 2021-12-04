#pragma once

#include "Cameras/Camera.h"
#include "Textures/Textures.h"
#include "Textures/Atlas.h"
#include "Shaders.h"

namespace Kross {
	struct Voxel;
	class VoxelRenderer
	{
		static bool s_bSceneBegan;
		static bool s_bBatch;
		static bool s_bInitiated;

	public:
		struct Stats
		{
			uint32_t DrawCount = 0;
			uint32_t QuadCount = 0;
		};

		VoxelRenderer() = delete;
		VoxelRenderer(VoxelRenderer&& other) = delete;
		VoxelRenderer& operator =(VoxelRenderer&& other) = delete;

		static const Stats& getStats();
		static void ResetStats();

		static void Init();
		static void Shutdown();

		static void Begin(Ref<Camera::Camera>& camera);
		static void DrawVoxel(const Voxel& params);
		static void End();

		static void SwitchShader(const Ref<Shader>& shader);
	};

}

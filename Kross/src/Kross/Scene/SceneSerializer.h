#pragma once

#include "Kross/Core/Core.h"
#include "Scene.h"

namespace Kross {
	class KAPI SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene> &scene);

		void Serialize(const char *filepath);
		bool Deserialize(const char *filepath);
		void SerializeRuntime(const char *filepath);
		bool DeserializeRuntime(const char *filepath);
	private:
		Ref<Scene> m_Scene;
	};
}

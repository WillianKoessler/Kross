#pragma once

#include "Kross/Core/Core.h"
#include "Scene.h"

namespace Kross {
	class KAPI SceneSerializer
	{
	public:
		SceneSerializer(Scene *scene);
		SceneSerializer(const Ref<Scene> &scene);

		void Serialize(const File &file);
		bool Deserialize(const File &file);
		void SerializeRuntime(const File &file);
		bool DeserializeRuntime(const File &file);
	private:
		Ref<Scene> m_Scene = nullptr;
		Scene *m_pScene = nullptr;
	};
}

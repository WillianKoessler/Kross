#include <Kross_pch.h>
#include "Kross/Core/Stack.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include "Components.h"
#include <yaml-cpp/yaml.h>
#include <filesystem>

namespace YAML {
	template<> struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4 &v)
		{
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			node.push_back(v.w);
			return node;
		}
		static bool decode(const Node &node, glm::vec4 &v)
		{
			if (!node.IsSequence() || node.size() > 4)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			v.w = node[3].as<float>();
			return true;
		}
	};
	template<> struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3 &v)
		{
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			return node;
		}
		static bool decode(const Node &node, glm::vec3 &v)
		{
			if (!node.IsSequence() || node.size() > 3)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			return true;
		}
	};
	Emitter &operator<<(Emitter &out, const glm::vec3 &v)
	{
		out << Flow << BeginSeq << v.x << v.y << v.z << EndSeq;
		return out;
	}
	Emitter &operator<<(Emitter &out, const glm::vec4 &v)
	{
		out << Flow << BeginSeq << v.r << v.g << v.b << v.a << EndSeq;
		return out;
	}
}

namespace Kross {
	SceneSerializer::SceneSerializer(Scene *scene)
		: m_pScene(scene)
	{
	}
	SceneSerializer::SceneSerializer(const Ref<Scene> &scene)
		: m_Scene(scene)
	{
	}
	static void SerializeEntity(YAML::Emitter &out, Entity e)
	{
		using namespace YAML;
		out << BeginMap;
		out << YAML::Key << "Entity" << Value << (uint32_t)e;
		if (e.Has<TagComponent>() == 1) {
			out << YAML::Key << "TagComponent";
			out << BeginMap;

			out << YAML::Key << "Tag" << Value << e.Get<TagComponent>()->Get();

			out << EndMap;
		}
		if (e.Has<TransformComponent>() == 1) {
			out << YAML::Key << "TransformComponent";
			out << BeginMap;

			auto transform = e.Get<TransformComponent>();
			out << YAML::Key << "Position" << Value << transform->Position;
			out << YAML::Key << "Rotation" << Value << transform->Rotation;
			out << YAML::Key << "Scale" << Value << transform->Scale;

			out << EndMap;
		}
		if (e.Has<SpriteComponent>() == 1) {
			out << YAML::Key << "SpriteComponent";
			out << BeginMap;

			out << YAML::Key << "Tint" << Value << e.Get<SpriteComponent>()->tint;
			const char *name = e.Get<SpriteComponent>()->texture->GetName();
			const char *path = e.Get<SpriteComponent>()->texture->GetPath();
			if (path != nullptr && name != nullptr) {
				out << YAML::Key << "Texture" << BeginMap;
				if (name != nullptr) out << YAML::Key << "Name" << Value << std::string(name);
				if (path != nullptr) out << YAML::Key << "Path" << Value << std::string(path);
				out << EndMap;
			}

			out << EndMap;
		}
		if (e.Has<CameraComponent>() == 1) {
			out << YAML::Key << "CameraComponent";
			{
				out << BeginMap; // <CameraComponent>
				auto component = e.Get<CameraComponent>();
				auto sceneCamera = component->camera;
				{
					auto previous = sceneCamera.GetProjType();
					out << YAML::Key << "Camera" << Value << BeginMap; // <Camera>
					out << YAML::Key << "ProjectionType" << Value << (int)sceneCamera.GetProjType();
					sceneCamera.SetProjType(SceneCamera::ProjectionType::Perspective);
					out << YAML::Key << "PerspectiveFOV" << Value << sceneCamera.GetPerspVerticalFOV();
					out << YAML::Key << "PerspectiveNear" << Value << sceneCamera.GetNearClip();
					out << YAML::Key << "PerspectiveFar" << Value << sceneCamera.GetFarClip();
					sceneCamera.SetProjType(SceneCamera::ProjectionType::Orthographic);
					out << YAML::Key << "OrthographicSize" << Value << sceneCamera.OrthoSize();
					out << YAML::Key << "OrthographicNear" << Value << sceneCamera.GetNearClip();
					out << YAML::Key << "OrthographicFar" << Value << sceneCamera.GetFarClip();
					out << EndMap; // </Camera>
				}
				out << YAML::Key << "FixedAspectRatio" << Value << component->fixedAspectRatio;
				out << EndMap; // </CameraComponent>
			}
		}
		if (e.Has<NativeScriptComponent>() == 1) {

		}
		out << EndMap;
	}
	void SceneSerializer::Serialize(const File &file)
	{
		Scene *scene = nullptr;
		if (m_pScene) scene = m_pScene;
		else if (m_Scene) scene = m_Scene.get();
		else {
			KROSS_ERROR("Trying to serialize a nullptr Scene.");
			return;
		}
		KROSS_ASSERT((m_pScene || m_Scene) && !(m_pScene && m_Scene), "Ambiguity found. Cannot have both pointers.");
		using namespace YAML;
		Emitter out;
		out << BeginMap;
		out << YAML::Key << "Scene" << Value << scene->m_strName;
		out << YAML::Key << "ActiveCameraID" << Value << (uint32_t)scene->m_PrimaryCameraID;
		out << YAML::Key << "Entities" << BeginSeq;
		scene->m_Registry.each([&](auto &e) {
			Entity entity{ e, scene };
			if (!entity) return;
			else SerializeEntity(out, entity);
			});
		KROSS_TRACE("Serializing Scene to file '{0}' ({1})\n\tpath: {2}", file.name, file.extension, file.path);
		//if (!std::filesystem::exists(std::filesystem::path(filepath)))
		//	std::filesystem::create_directory(std::filesystem::path(filepath));
		std::ofstream fileData(file.path, std::ofstream::trunc);
		fileData << out.c_str();
		fileData.close();
	}
	bool SceneSerializer::Deserialize(const File &file)
	{
		Scene *scene = nullptr;
		if (m_pScene) scene = m_pScene;
		else if (m_Scene) scene = m_Scene.get();
		else {
			KROSS_ERROR("Trying to serialize a nullptr Scene.");
			return false;
		}
		KROSS_ASSERT((m_pScene || m_Scene) && !(m_pScene && m_Scene), "Ambiguity found. Cannot have both pointers.");
		std::ifstream fileData(file.path);
		std::stringstream ss;
		ss << fileData.rdbuf();

		using namespace YAML;
		Node data = Load(ss.str());
		if (!data["Scene"]) return false;

		std::string sceneName = data["Scene"].as<std::string>();
		scene->SetName(sceneName.c_str());

		Entity activeCamera = { (entt::entity)data["ActiveCameraID"].as<uint32_t>(), scene };
		scene->m_PrimaryCameraID = entt::null;

		auto entities = data["Entities"];
		if (entities)
			for (auto entity : entities) {
				uint32_t uuid = entity["Entity"].as<uint32_t>();

				auto tagComponent = entity["TagComponent"];
				std::string tag = "";
				if (tagComponent)
					tag = tagComponent["Tag"].as<std::string>();
				Entity newEntity = scene->CreateEntity(tag.c_str());

				auto transform = entity["TransformComponent"];
				if (transform) {
					auto cmp = newEntity.Add<TransformComponent>();
					cmp->Position = transform["Position"].as<glm::vec3>();
					cmp->Rotation = transform["Rotation"].as<glm::vec3>();
					cmp->Scale = transform["Scale"].as<glm::vec3>();
				}

				auto sprite = entity["SpriteComponent"];
				if (sprite) {
					auto cmp = newEntity.Add<SpriteComponent>();
					cmp->tint = sprite["Tint"].as<glm::vec4>();
					auto hasTexture = sprite["Texture"];
					if (hasTexture) {
						std::string name, path;
						auto hasName = hasTexture["Name"];
						if (hasName) name = hasName.as<std::string>();
						auto hasPath = hasTexture["Path"];
						if (hasPath) path = hasPath.as<std::string>();
						if (hasName && hasPath)
							cmp->texture = Stack<Texture::T2D>::Get(name.c_str(), path.c_str());
						else if (hasName && !hasPath)
							cmp->texture = Stack<Texture::T2D>::Get(name.c_str());
						else if (!hasName && hasPath)
							cmp->texture = Stack<Texture::T2D>::Load(path.c_str());
					}
				}
				auto cameraNode = entity["CameraComponent"];
				if (cameraNode) {
					auto cmp = newEntity.Add<CameraComponent>();
					auto &sceneCamera = cameraNode["Camera"];
					cmp->camera.SetProjType(SceneCamera::ProjectionType::Perspective);
					cmp->camera.SetPerspVerticalFOV(sceneCamera["PerspectiveFOV"].as<float>());
					cmp->camera.SetNearClip(sceneCamera["PerspectiveNear"].as<float>());
					cmp->camera.SetFarClip(sceneCamera["PerspectiveFar"].as<float>());

					cmp->camera.SetProjType(SceneCamera::ProjectionType::Orthographic);
					cmp->camera.SetOrthoSize(sceneCamera["OrthographicSize"].as<float>());
					cmp->camera.SetNearClip(sceneCamera["OrthographicNear"].as<float>());
					cmp->camera.SetFarClip(sceneCamera["OrthographicFar"].as<float>());

					cmp->camera.SetProjType((SceneCamera::ProjectionType)sceneCamera["ProjectionType"].as<int>());

					cmp->fixedAspectRatio = cameraNode["FixedAspectRatio"].as<bool>();
				}
			}

		return true;
	}
	void SceneSerializer::SerializeRuntime(const File &file)
	{
		//Not Implemented
		KROSS_ASSERT(false);
	}
	bool SceneSerializer::DeserializeRuntime(const File &file)
	{
		//Not Implemented
		KROSS_ASSERT(false);
		return false;
	}
}

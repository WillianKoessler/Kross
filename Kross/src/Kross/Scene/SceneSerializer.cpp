#include <Kross_pch.h>
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
	SceneSerializer::SceneSerializer(const Ref<Scene> &scene)
		: m_Scene(scene)
	{
	}
	static void SerializeEntity(YAML::Emitter &out, Entity e)
	{
		using namespace YAML;
		out << BeginMap;
		out << Key << "Entity" << Value << (uint32_t)e;
		if (e.Has<TagComponent>() == 1) {
			out << Key << "TagComponent";
			out << BeginMap;

			out << Key << "Tag" << Value << e.Get<TagComponent>()->Get();

			out << EndMap;
		}
		if (e.Has<TransformComponent>() == 1) {
			out << Key << "TransformComponent";
			out << BeginMap;

			auto transform = e.Get<TransformComponent>();
			out << Key << "Position" << Value << transform->Position;
			out << Key << "Rotation" << Value << transform->Rotation;
			out << Key << "Scale" << Value << transform->Scale;

			out << EndMap;
		}
		if (e.Has<SpriteComponent>() == 1) {
			out << Key << "SpriteComponent";
			out << BeginMap;

			out << Key << "Tint" << Value << e.Get<SpriteComponent>()->tint;

			out << EndMap;
		}
		if (e.Has<CameraComponent>() == 1) {
			out << Key << "CameraComponent";
			{
				out << BeginMap; // <CameraComponent>
				auto component = e.Get<CameraComponent>();
				auto sceneCamera = component->camera;
				{
					auto previous = sceneCamera.GetProjType();
					out << Key << "Camera" << Value << BeginMap; // <Camera>
					out << Key << "ProjectionType" << Value << (int)sceneCamera.GetProjType();
					sceneCamera.SetProjType(SceneCamera::ProjectionType::Perspective);
					out << Key << "PerspectiveFOV" << Value << sceneCamera.GetPerspVerticalFOV();
					out << Key << "PerspectiveNear" << Value << sceneCamera.GetNearClip();
					out << Key << "PerspectiveFar" << Value << sceneCamera.GetFarClip();
					sceneCamera.SetProjType(SceneCamera::ProjectionType::Orthographic);
					out << Key << "OrthographicSize" << Value << sceneCamera.OrthoSize();
					out << Key << "OrthographicNear" << Value << sceneCamera.GetNearClip();
					out << Key << "OrthographicFar" << Value << sceneCamera.GetFarClip();
					out << EndMap; // </Camera>
				}
				out << Key << "FixedAspectRatio" << Value << component->fixedAspectRatio;
				out << EndMap; // </CameraComponent>
			}
		}
		if (e.Has<NativeScriptComponent>() == 1) {

		}
		out << EndMap;
	}
	void SceneSerializer::Serialize(const char *filepath)
	{
		using namespace YAML;
		Emitter out;
		out << BeginMap;
		out << Key << "Scene" << Value << "Untitled"; // TODO: create Scene m_Name
		out << Key << "ActiveCameraID" << Value << (uint32_t)m_Scene->m_PrimaryCameraID;
		out << Key << "Entities" << BeginSeq;
		m_Scene->m_Registry.each([&](auto &e) {
			Entity entity((uint32_t)e, m_Scene.get());
			if (!entity) return;
			else SerializeEntity(out, entity);
			});
		KROSS_TRACE("Serializing Scene to '{0}'", filepath);
		//if (!std::filesystem::exists(std::filesystem::path(filepath)))
		//	std::filesystem::create_directory(std::filesystem::path(filepath));
		std::ofstream file(filepath);
		file << out.c_str();
		file.close();
	}
	bool SceneSerializer::Deserialize(const char *filepath)
	{
		KROSS_TRACE("Deserializing Scene from '{0}'", filepath);
		std::ifstream file(filepath);
		std::stringstream ss;
		ss << file.rdbuf();

		using namespace YAML;
		Node data = Load(ss.str());
		if (!data["Scene"]) return false;

		std::string sceneName = data["Scene"].as<std::string>();

		Entity activeCamera = { data["ActiveCameraID"].as<uint32_t>(), m_Scene.get() };
		m_Scene->m_PrimaryCameraID = entt::null;

		auto entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {
				uint32_t uuid = entity["Entity"].as<uint32_t>();

				auto tagComponent = entity["TagComponent"];
				std::string tag = "";
				if (tagComponent)
					tag = tagComponent["Tag"].as<std::string>();
				Entity newEntity = m_Scene->CreateEntity(tag.c_str());

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
		}
		return true;
	}
	void SceneSerializer::SerializeRuntime(const char *filepath)
	{
		//Not Implemented
		KROSS_ASSERT(false);
	}
	bool SceneSerializer::DeserializeRuntime(const char *filepath)
	{
		//Not Implemented
		KROSS_ASSERT(false);
		return false;
	}
}

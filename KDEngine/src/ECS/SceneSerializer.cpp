#include "ECS/SceneSerializer.h"

#include "ECS/KDEntity.h"
#include "ECS/Components.h"
#include "Graphics/GeoPrimitives.h"

#include <yaml-cpp/yaml.h>
#include <DirectXMath.h>
#include <fstream>

namespace YAML
{
	template<>
	struct convert<DirectX::XMFLOAT3>
	{
		static Node encode(const DirectX::XMFLOAT3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}
		static bool decode(const Node& node, DirectX::XMFLOAT3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<DirectX::XMFLOAT4>
	{
		static Node encode(const DirectX::XMFLOAT4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		static bool decode(const Node& node, DirectX::XMFLOAT4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace KDE
{
	YAML::Emitter& operator << (YAML::Emitter& out, const DirectX::XMFLOAT3& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator << (YAML::Emitter& out, const DirectX::XMFLOAT4& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(KDScene* scene)
		: m_Scene(scene)
	{}
	// SceneSerializer::SceneSerializer(const Ref<Scene>& scene, const Ref<EditorCamera>& editCamera)
	// 	: m_Scene(scene), m_EditorCamera(editCamera)
	// {}

	static void SerializeEntity(YAML::Emitter& out, KDEntity& ent)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "12837192831273";

		if (ent.HasComponent<CS::TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = ent.GetComponent<CS::TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}
		if (ent.HasComponent<CS::RenderComponent>())
		{
			out << YAML::Key << "RenderComponent";
			out << YAML::BeginMap;

			auto& rc = ent.GetComponent<CS::RenderComponent>();
			out << YAML::Key << "Position" << YAML::Value << rc.Mesh->Transform.Position;
			out << YAML::Key << "Rotation" << YAML::Value << rc.Mesh->Transform.Rotation;
			out << YAML::Key << "Scaling" << YAML::Value << rc.Mesh->Transform.Scaling;

			out << YAML::Key << "Mesh" << YAML::Value << rc.Mesh->Name;

			out << YAML::EndMap;
		}
		if (ent.HasComponent<CS::PointLightComponent>())
		{
			out << YAML::Key << "PointLightComponent";
			out << YAML::BeginMap;

			auto& plc = ent.GetComponent<CS::PointLightComponent>();
			out << YAML::Key << "Ambient" << YAML::Value << plc.LightCBuffer.Ambient;
			out << YAML::Key << "AttenuateConst" << YAML::Value << plc.LightCBuffer.AttenuateConst;
			out << YAML::Key << "AttenuateLatency" << YAML::Value << plc.LightCBuffer.AttenuateLatency;
			out << YAML::Key << "AttenuateQuad" << YAML::Value << plc.LightCBuffer.AttenuateQuad;
			out << YAML::Key << "DiffuseColor" << YAML::Value << plc.LightCBuffer.DiffuseColor;
			out << YAML::Key << "DiffuseIntensity" << YAML::Value << plc.LightCBuffer.DiffuseIntensity;
			out << YAML::Key << "LightPos" << YAML::Value << plc.LightCBuffer.LightPos;

			out << YAML::EndMap;
		}
		//	if (ent.HasComponent<CameraComponent>())
		//	{
		//		out << YAML::Key << "CameraComponent";
		//		out << YAML::BeginMap; // CameraComponent
		//
		//		auto& cameraComponent = ent.GetComponent<CameraComponent>();
		//		auto& camera = cameraComponent.Camera;
		//
		//		out << YAML::Key << "Camera" << YAML::Value;
		//		out << YAML::BeginMap; // Camera
		//		out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
		//		out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspFOV();
		//		out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspNearClip();
		//		out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspFarClip();
		//		out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthoSize();
		//		out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthoNearClip();
		//		out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthoFarClip();
		//		out << YAML::EndMap; // Camera
		//
		//		out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
		//		out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;
		//
		//		out << YAML::EndMap; // CameraComponent
		//	}
		//	if (ent.HasComponent<SpriteRendererComponent>())
		//	{
		//		out << YAML::Key << "SpriteRendererComponent";
		//		out << YAML::BeginMap; // SpriteRendererComponent
		//
		//		auto& spriteRendererComponent = ent.GetComponent<SpriteRendererComponent>();
		//		out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
		//
		//		out << YAML::EndMap; // SpriteRendererComponent
		//	}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& src)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled Scene";

		//	out << YAML::Key << "EditorCamera" << YAML::Value << YAML::BeginMap;
		//	out << YAML::Key << "Position" << YAML::Value << m_EditorCamera->GetPosition();
		//	out << YAML::Key << "Orientation" << YAML::Value << m_EditorCamera->GetOrientation();
		//	out << YAML::Key << "PerspectiveFOV" << YAML::Value << m_EditorCamera->GetFOV();
		//	out << YAML::Key << "PerspectiveNear" << YAML::Value << m_EditorCamera->GetNearClip();
		//	out << YAML::Key << "PerspectiveFar" << YAML::Value << m_EditorCamera->GetFarClip();
		//	out << YAML::EndMap;

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each(
			[&](auto entID)
			{
				KDEntity ent = { entID, m_Scene };
				SerializeEntity(out, ent);
			}
		);
		out << YAML::EndSeq << YAML::EndMap;

		std::ofstream fout(src);
		fout << out.c_str();
	}
	bool SceneSerializer::Deserialize(const std::string& src)
	{
		std::ifstream stream(src);
		if (!stream.is_open()) throw std::exception((std::string("Failed to open file:") + src).c_str());

		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		//	if (auto editCameraData = data["EditorCamera"]; editCameraData.IsDefined())
		//	{
		//		DirectX::XMFLOAT3 editCamPos = editCameraData["Position"].as<DirectX::XMFLOAT3>();
		//		DirectX::XMFLOAT3 editCamOri = editCameraData["Orientation"].as<DirectX::XMFLOAT3>();
		//		float editCamFov = editCameraData["PerspectiveFOV"].as<float>();
		//		float editCamNear = editCameraData["PerspectiveNear"].as<float>();
		//		float editCamFar = editCameraData["PerspectiveFar"].as<float>();
		//		m_EditorCamera->SetPosition(editCamPos);
		//		m_EditorCamera->SetOrientation(editCamOri);
		//		m_EditorCamera->SetFOV(editCamFov);
		//		m_EditorCamera->SetNearClip(editCamNear);
		//		m_EditorCamera->SetFarClip(editCamFar);
		//	}

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto ent : entities)
			{
				uint64_t uuid = ent["Entity"].as<uint64_t>();

				std::string name;
				auto tagComp = ent["TagComponent"];
				if (tagComp)
					name = tagComp["Tag"].as<std::string>();

				KDEntity deserEntity = m_Scene->CreateEntity(name);

				auto renderComp = ent["RenderComponent"];
				if (renderComp)
				{
					auto& tc = deserEntity.GetComponent<CS::RenderComponent>();
					tc.Mesh->Transform.Position = renderComp["Position"].as<DirectX::XMFLOAT3>();
					tc.Mesh->Transform.Rotation = renderComp["Rotation"].as<DirectX::XMFLOAT3>();
					tc.Mesh->Transform.Scaling = renderComp["Scaling"].as<DirectX::XMFLOAT3>();

					auto meshName = renderComp["Mesh"].as<std::string>();
					auto& pMesh = tc.Mesh;

					if (meshName == "Cube")
					{
						pMesh = std::make_shared<KDMesh>(GP::Cube::MakeIndependent());
					}
					else if (meshName == "Cone")
					{
						pMesh = std::make_shared<KDMesh>(GP::Cone::MakeIndependent(24));
					}
					else if (meshName == "Plane")
					{
						pMesh = std::make_shared<KDMesh>(GP::Plane::Make());
					}
					else if (meshName == "Prism")
					{
						pMesh = std::make_shared<KDMesh>(GP::Prism::MakeIndependent(24));
					}
					else if (meshName == "Sphere")
					{
						pMesh = std::make_shared<KDMesh>(GP::Sphere::Make());
					}
				}
				auto plComp = ent["PointLightComponent"];
				if (plComp)
				{
					auto& plc = deserEntity.AddComponent<CS::PointLightComponent>();
					plc.LightCBuffer.Ambient = plComp["Ambient"].as<DirectX::XMFLOAT3>();
					plc.LightCBuffer.AttenuateConst = plComp["AttenuateConst"].as<float>();
					plc.LightCBuffer.AttenuateLatency = plComp["AttenuateLatency"].as<float>();
					plc.LightCBuffer.AttenuateQuad = plComp["AttenuateQuad"].as<float>();
					plc.LightCBuffer.DiffuseColor = plComp["DiffuseColor"].as<DirectX::XMFLOAT3>();
					plc.LightCBuffer.DiffuseIntensity = plComp["DiffuseIntensity"].as<float>();
					plc.LightCBuffer.LightPos = plComp["LightPos"].as<DirectX::XMFLOAT3>();
				}
				//	auto camDiffuseIntensityComp = ent["CameraComponent"];
				//	if (camCLightPos;omp)
			//	{
			//		auto& cc = deserEntity.AddComponent<CameraComponent>();
			//
			//		const auto& cameraProps = camComp["Camera"];
			//		cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
			//
			//		cc.Camera.SetPerspFOV(cameraProps["PerspectiveFOV"].as<float>());
			//		cc.Camera.SetPerspNearClip(cameraProps["PerspectiveNear"].as<float>());
			//		cc.Camera.SetPerspFarClip(cameraProps["PerspectiveFar"].as<float>());
			//
			//		cc.Camera.SetOrthoSize(cameraProps["OrthographicSize"].as<float>());
			//		cc.Camera.SetOrthoNearClip(cameraProps["OrthographicNear"].as<float>());
			//		cc.Camera.SetOrthoFarClip(cameraProps["OrthographicFar"].as<float>());
			//
			//		cc.Primary = camComp["Primary"].as<bool>();
			//		cc.FixedAspectRatio = camComp["FixedAspectRatio"].as<bool>();
			//	}
			}
		}

		return true;
	}
}
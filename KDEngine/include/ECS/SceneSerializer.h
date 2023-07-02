#pragma once

#include <string>

#include "KDScene.h"

namespace KDE
{
	class SceneSerializer
	{
	public:
		SceneSerializer(KDScene* scene);
	// TODO:	SceneSerializer(const Scene>& scene, const Ref<EditorCamera>& editCamera);

		void Serialize(const std::string& src);
		bool Deserialize(const std::string& src);

	//	TODO:
	//	static void SerializeRuntime(const std::string& src);
	//	static void DeserializeRuntime(const std::string& src);
	private:
		KDScene* m_Scene;
	};
}
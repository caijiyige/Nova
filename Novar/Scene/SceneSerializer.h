#pragma once


#include "Novar/Scene/Entity.h"
#include "Novar/Scene/Scene.h"

namespace NV
{
    class SceneSerializer
    {
    public:
        SceneSerializer(const std::shared_ptr<Scene>& scene);
        ~SceneSerializer() = default;

        void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
        void SetSelectedScene(const std::shared_ptr<Scene>& spScene);

    private:
        std::shared_ptr<Scene> m_Scene;    
    };
}
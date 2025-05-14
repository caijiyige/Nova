#pragma once

namespace NV
{
    class Scene;
    class Entity;
    class SceneHierachyPanel
    {
    public:
        SceneHierachyPanel() = default;
        SceneHierachyPanel(const std::shared_ptr<Scene>& context);
    
        void OnImGuiRender();
        void SetContext(const std::shared_ptr<Scene>& context) { m_spContext = context; }
        void SetSelectedEntity(std::shared_ptr<Entity> entity) { m_spSelectionContextEntity = entity; }
        std::shared_ptr<Entity> GetSelectedEntity() const { return m_spSelectionContextEntity; }
        

    private:
        template<typename T>
        void DisplayAddComponentEntry(const std::string& entryName);
	    void DrawEntityNode(const std::shared_ptr<Entity>& entity);
	    void DrawComponents(const std::shared_ptr<Entity>& entity);

    private:
        std::shared_ptr<Scene> m_spContext;
        std::shared_ptr<Entity> m_spSelectionContextEntity;
       
    };
}
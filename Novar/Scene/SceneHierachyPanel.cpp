#include "Novar/Scene/SceneHierachyPanel.h"
#include "imgui.h"
#include "Novar/Scene/Scene.h"
#include "Novar/Scene/Entity.h"
#include <filesystem>
namespace NV
{
    
    SceneHierachyPanel::SceneHierachyPanel(const std::shared_ptr<Scene> &context)
        : m_spContext(context)
    {
        m_spSelectionContextEntity = nullptr;
    }
    void SceneHierachyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");
        if(m_spContext)
        {
            auto spRegistry = m_spContext->GetRegistry();
            spRegistry->each([&](auto entityID)
            {
                DrawEntityNode(std::make_shared<Entity>(spRegistry, entityID));
            });

            if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            {
                m_spSelectionContextEntity = nullptr;
            }
            if (ImGui::BeginPopupContextWindow(0, 1))
            {
                if (ImGui::MenuItem("Create Entity"))
                {
                    m_spContext->CreateEntity("New Entity");
                }
                ImGui::EndPopup();
            }
        }
        
        ImGui::End();



        ImGui::Begin("Properties");
        if (m_spSelectionContextEntity && m_spContext)
        {
            ImGui::Text("Entity: %s", m_spSelectionContextEntity->GetComponent<TagComponent>().Tag.c_str());
            DrawComponents(m_spSelectionContextEntity);
            if (ImGui::Button("Add Component"))
            {
                ImGui::OpenPopup("AddComponent");
            }
            if (ImGui::BeginPopup("AddComponent"))
            {
                if (ImGui::MenuItem("Camera"))
                {
                    m_spSelectionContextEntity->AddComponent<CameraComponent>();
                }
                if (ImGui::MenuItem("Sprite"))
                {
                    m_spSelectionContextEntity->AddComponent<SpriteRendererComponent>();
                }
                if (ImGui::MenuItem("Transform"))
                {
                    m_spSelectionContextEntity->AddComponent<TransformComponent>();
                }
                ImGui::EndPopup();
            }
        }
        else
        {
            ImGui::Text("No Entity Selected");
        }
    }
    void SceneHierachyPanel::DrawEntityNode(const std::shared_ptr<Entity>& entity)
    {
        auto& tagComponent = entity->GetComponent<TagComponent>();
        ImGuiTreeNodeFlags treeNodeFlags = ((m_spSelectionContextEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

        bool nodeOpen = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)(*entity), treeNodeFlags,tagComponent.Tag.c_str());
        if (ImGui::IsItemClicked())
        {
            m_spSelectionContextEntity = entity;
        }

        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
            {
                entityDeleted = true;
            }
            ImGui::EndPopup();
        }

        if (nodeOpen)
        {
            ImGui::TreePop();
        }

        if (entityDeleted)
        {
            
            if (m_spSelectionContextEntity == entity)
            {
                m_spSelectionContextEntity = {};
            }
            m_spContext->RemoveEntity(entity);
        }
        
    }

    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
    {
        ImGui::Text(label.c_str());

        ImGui::SameLine(columnWidth);
        ImGui::DragFloat3(label.c_str(), glm::value_ptr(values), 0.1f);
        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            values = { resetValue, resetValue, resetValue };
        }
    }

    template<typename T, typename UIFounction>
    static void DrawComponent(const std::string& label,std::shared_ptr<Entity> entity , UIFounction drawFunction)
    {
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        if (entity->HasComponent<T>())
        {
            auto& component = entity->GetComponent<T>();
            ImGui::PushID(label.c_str());
            
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, label.c_str());
            ImGui::SameLine();
            if (ImGui::Button("+"))
            {
                ImGui::OpenPopup("componentSettings");
            }

            bool removeComponent = false;
            if (ImGui::BeginPopup("componentSettings"))
            {
                if (ImGui::MenuItem("Remove Component"))
                {
                    removeComponent = true;
                }
                ImGui::EndPopup();
            }
            
            if(open)
            {
                drawFunction(component);
                ImGui::TreePop();
            }

            if (removeComponent)
            {
                entity->RemoveComponent<T>();
            }
        }
    }


    void SceneHierachyPanel::DrawComponents(const std::shared_ptr<Entity>& entity)
    {
    
        if (entity->HasComponent<TagComponent>())
        {
            auto& tag = entity->GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
        }


        DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
            {
                DrawVec3Control("Translation", component.Translation);
                glm::vec3 rotation = glm::degrees(component.Rotation);
                DrawVec3Control("Rotation", rotation);
                component.Rotation = glm::radians(rotation);
                DrawVec3Control("Scale", component.Scale, 1.0f);
            });

        DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
            {
                std::string ProjectionTypeStrings[] = { "Perspective", "Orthographic" };
                auto sCurrentProjectionType = ProjectionTypeStrings[static_cast<unsigned int>(component.Camera.GetProjectionType())];

                if (ImGui::BeginCombo("Projection", sCurrentProjectionType.c_str()))
                {
                    for (size_t i = 0; i < 2; i++)
                    {
                        bool bSelected = sCurrentProjectionType == ProjectionTypeStrings[i];
                        if (ImGui::Selectable(ProjectionTypeStrings[i].c_str(), bSelected))
                        {
                            component.Camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(i));
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }

                switch (component.Camera.GetProjectionType())
                {
                case SceneCamera::ProjectionType::Perspective:
                {
                    
                    float fPerFov = component.Camera.GetFieldOfView();
                    if (ImGui::DragFloat("FOV", &fPerFov))
                    {
                        component.Camera.SetFieldOfView(fPerFov);
                    }

                    float fOrthoNear = component.Camera.GetNearClip();
                    if (ImGui::DragFloat("Near", &fOrthoNear))
                    {
                        component.Camera.SetNearClip(fOrthoNear);
                    }

                    float fOrthoFar = component.Camera.GetFarClip();
                    if (ImGui::DragFloat("Far", &fOrthoFar))
                    {
                        component.Camera.SetFarClip(fOrthoFar);
                    }
                    break;
                }
                case SceneCamera::ProjectionType::Orthographic:
                {
                   

                    float fOrthoSize = component.Camera.GetOrthographicSize();
                    if (ImGui::DragFloat("Size", &fOrthoSize))
                    {
                        component.Camera.SetOrthographicSize(fOrthoSize);
                    }

                    float fOrthoNear = component.Camera.GetNearClip();
                    if (ImGui::DragFloat("Near", &fOrthoNear))
                    {
                        component.Camera.SetNearClip(fOrthoNear);
                    }

                    float fOrthoFar = component.Camera.GetFarClip();
                    if (ImGui::DragFloat("Far", &fOrthoFar))
                    {
                        component.Camera.SetFarClip(fOrthoFar);
                    }
                    break;
                }

                default:
                    break;
                }

            });
       

        DrawComponent<SpriteRendererComponent>("Sprite Render", entity, [](auto& component)
            {
                auto& spQuadPrimitive = component;
                ImGui::ColorEdit4("Color", glm::value_ptr(spQuadPrimitive.Color));
                ImGui::Button("Texture", ImVec2(100.f, 0.0f));
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
                    {
                        const wchar_t* path = (const wchar_t*)payload->Data;
                        std::filesystem::path filePath(path);
                        std::shared_ptr<Texture2D> texture = Texture2D::Create(filePath.generic_string());
                        if (texture->IsLoaded())
                        {
                            component.Texture = texture;
                        }
                        
                    }
                    ImGui::EndDragDropTarget();
                }

                ImGui::DragFloat("Tiling Facto", &spQuadPrimitive.Tiling, 0.1f, 0.f, 10.f);
            });

        DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component)
            {
                auto sCurrentBodyType = BodyTypeStrings[static_cast<unsigned int>(component.Type)];

                if (ImGui::BeginCombo("Body Type", sCurrentBodyType.c_str()))
                {
                    for (size_t i = 0; i < BodyTypeStrings.size(); i++)
                    {
                        bool bSelected = sCurrentBodyType == BodyTypeStrings[i];
                        if (ImGui::Selectable(BodyTypeStrings[i].c_str(), bSelected))
                        {
                            component.Type = static_cast<Rigidbody2DComponent::BodyType>(i);
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();

                }

                ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
            });

        DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
            {
                ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
                ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
                ImGui::DragFloat("Density", &component.Density, 0.01f, 0.f, 1.f);
                ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.f, 1.f);
                ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.f, 1.f);
                ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.f);
            });
       
    }


    template<typename T>
    void SceneHierachyPanel::DisplayAddComponentEntry(const std::string& entryName)
    {
        if (!m_spSelectionContextEntity->HasComponent<T>())
        {
            if (ImGui::MenuItem(entryName.c_str()))
            {
                m_spSelectionContextEntity->AddComponent<T>();
                ImGui::CloseCurrentPopup();
            }
        }
    }
}
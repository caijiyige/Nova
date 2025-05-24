#include "Novar/Scene/SceneHierachyPanel.h"
#include "imgui.h"
#include "imgui_internal.h"
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
        if (m_spContext && m_spSelectionContextEntity )
        {
            DrawComponents(m_spSelectionContextEntity);
        }
        else
        {
            ImGui::Text("No Entity Selected");
        }
        ImGui::End();
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
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
        

    template<typename T, typename UIFounction>
    static void DrawComponent(const std::string& label,const std::shared_ptr<Entity> entity , UIFounction drawFunction)
    {
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        if (entity->HasComponent<T>())
        {
            auto& component = entity->GetComponent<T>();
            
            
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
        ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			//DisplayAddComponentEntry<ScriptComponent>("Script");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<CircleRendererComponent>("Circle Renderer");
			DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody 2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
			//DisplayAddComponentEntry<CircleCollider2DComponent>("Circle Collider 2D");
			//DisplayAddComponentEntry<TextComponent>("Text Component");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

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
                //auto& spQuadPrimitive = component;
                ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
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
                // else
                // {
                //         std::shared_ptr<Texture2D> texture = Texture2D::Create(TextureSpecification());
                //         uint32_t whiteTextureData = 0xffffffff;
                //         texture->SetData(&whiteTextureData,sizeof(uint32_t));
                //         component.Texture = texture;
                // }

                ImGui::DragFloat("Tiling Facto", &component.Tiling, 0.1f, 0.0f, 10.f);
            });

        DrawComponent<CircleRendererComponent>("Circle Render", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
			ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
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
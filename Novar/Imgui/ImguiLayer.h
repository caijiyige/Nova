#pragma once

#include "Novar/Core/Layer.h"

namespace NV
{
    class NOVA_API ImguiLayer : public Layer
    {
        public:
            ImguiLayer();
            ~ImguiLayer();

            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnUpdate(Timestep ts) override;
            virtual void OnEvent(Event& e) override;
            //virtual void OnImGuiRender() override;
            
            void Begin();
            void End();

            void BlockEvents(bool block) { m_BlockEvent = block; }
        
        private:
        bool m_BlockEvent = true;
        float m_Time = 0.0f;
    };
}
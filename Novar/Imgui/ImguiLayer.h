#pragma once

#include "Layer.h"



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

            virtual void OnImGuiRender() override;
            
            void Begin();
            void End();
        
        private:
            float m_Time = 0.0f;
    };
}
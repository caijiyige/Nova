#pragma once

#include "Novar/Core/Core.h"
#include "Novar/Core/Timestep.h"
#include "Novar/Events/Event.h"


namespace NV
{
    class NOVA_API Layer
    {
        public:
            Layer(const std::string& name = "Layer");
            virtual ~Layer();

            virtual void OnAttach() {}
            virtual void OnDetach() {}
            virtual void OnUpdate(Timestep ts) {}
            virtual void OnImGuiRender() {}
            virtual void OnEvent(Event& e) {}

            inline const std::string& GetName() const { return m_DebugName; }
        protected:
            std::string m_DebugName;
    };
}
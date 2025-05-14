#pragma once

#include "Novar/Events/Event.h"


namespace NV{

   
    class MouseScrollEvent : public Event
    {
        public:
            MouseScrollEvent(float xOffset, float yOffset)
                : m_XOffset(xOffset), m_YOffset(yOffset) {}
            inline int GetXOffset() const { return m_XOffset; }
            inline int GetYOffset() const { return m_YOffset; }
            EVENT_CLASS_TYPE(MouseScrolled)
            EVENT_CLASS_CATEGORY(EventCategory::Mouse)
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "MouseScrolledEvent" << ": " << m_XOffset << ", " << m_YOffset;
                return ss.str();
            }
    
        private:
                /* data */
                float m_XOffset;
                float m_YOffset;
    };
    class MouseButtonEvent : public Event
    {
        public:
            inline int GetMouseButton() const { return m_Button; }
            EVENT_CLASS_CATEGORY(EventCategory::Mouse)
        protected:
            MouseButtonEvent(int button)
                : m_Button(button) {}
            int m_Button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
        public:
            MouseButtonPressedEvent(int button)
                : MouseButtonEvent(button) {}
            EVENT_CLASS_TYPE(MouseButtonPressed)
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "MouseButtonPressedEvent" << ": " << m_Button;
                return ss.str();
            }
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
        public:
            MouseButtonReleasedEvent(int button)
                : MouseButtonEvent(button) {}
            EVENT_CLASS_TYPE(MouseButtonReleased)
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "MouseButtonReleasedEvent" << ": " << m_Button;
                return ss.str();
            }
    };

    class MouseMovedEvent : public Event
    {
        public:
            MouseMovedEvent(float x, float y)
                : m_X(x), m_Y(y) {}
            EVENT_CLASS_TYPE(MouseMoved)
            EVENT_CLASS_CATEGORY(EventCategory::Mouse)

            
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "MouseMovedEvent" << ": " << m_X << ", " << m_Y;
                return ss.str();
            }

            inline float GetX() const { return m_X; }
            inline float GetY() const { return m_Y; }
        private:
            float m_X, m_Y;
    };
}
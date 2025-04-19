#pragma once

#include "Event.h"

namespace NV
{

    class  WindowCloseEvent : public Event
    {
        public:
            EVENT_CLASS_TYPE(WindowClose)
            EVENT_CLASS_CATEGORY(EventCategory::Application)
    };

    class  WindowResizeEvent : public Event
    {
        public:
        WindowResizeEvent(int width, int height)
            : m_Width(width), m_Height(height) {}

        inline int GetWidth() const { return m_Width; }
        inline int GetHeight() const { return m_Height; }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategory::Application)
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent" << ": " << m_Width << ", " << m_Height;
            return ss.str();
        }
        private:
            int m_Width;
            int m_Height;
    };

    class WindowFocusEvent : public Event
    {
        public:
            WindowFocusEvent() = default;
            EVENT_CLASS_TYPE(WindowFocus)
            EVENT_CLASS_CATEGORY(EventCategory::Application)
    };

    class AppTickEvent : public Event
    {
        public:
            AppTickEvent() = default;
            EVENT_CLASS_TYPE(AppTick)
            EVENT_CLASS_CATEGORY(EventCategory::Application)
    };

    class AppUpdateEvent : public Event
    {
        public:
            AppUpdateEvent() = default;
            EVENT_CLASS_TYPE(AppUpdate)
            EVENT_CLASS_CATEGORY(EventCategory::Application)
    };
    class AppRenderEvent : public Event
    {
        public:
            AppRenderEvent() = default;
            EVENT_CLASS_TYPE(AppRender)
            EVENT_CLASS_CATEGORY(EventCategory::Application)
    };
    
    
    

}
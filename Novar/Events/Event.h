#pragma once

#include "Novar/Core/Base.h"

namespace NV
{
   
    // Event types
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    // Event categories
    enum class EventCategory
    {
        None = 0,
        Application= BIT(0),
        Input = BIT(1),
        Keyboard = BIT(2),
        Mouse = BIT(3),
        MouseButton = BIT(4)
    };

    // 重载位运算符 |
    inline constexpr int operator|(EventCategory lhs, EventCategory rhs) {
    return static_cast<int>(lhs) | static_cast<int>(rhs);
    }

    // Event base class
    #define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                                   virtual EventType GetEventType() const override { return GetStaticType(); }\
                                   virtual const char* GetName() const override { return #type; }
                                 


    #define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return (int)category; }

    // Event base class

    class Event
    {

        public:
            virtual ~Event() = default;
            virtual const char* GetName() const = 0;
            virtual EventType GetEventType() const = 0;
            virtual int GetCategoryFlags() const = 0;
            virtual std::string ToString() const { return GetName(); };
            

            inline bool IsInCategory(EventCategory category)
            {
                return GetCategoryFlags() & (int)category;
            }

            bool m_Handled = false;
    };


    // Event dispatcher
    class EventDispatcher
    {
        public:
            EventDispatcher(Event& event) : m_Event(event) {}

            template<typename T>
            using EventFn = std::function<bool(T&)>;

            template<typename T>
            bool Dispatch(EventFn<T> func)
            {
                if (m_Event.GetEventType() == T::GetStaticType())
                {
                    m_Event.m_Handled = func(*(T*)&m_Event);
                    return true;
                }
                return false;
            }
        private:
            Event& m_Event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}
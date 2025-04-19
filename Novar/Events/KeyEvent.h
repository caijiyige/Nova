#pragma once

#include "Event.h"


namespace NV
{
    class KeyEvent : public Event
    {
        public:
            int GetKeyCode() const { return m_KeyCode; }
            EVENT_CLASS_CATEGORY(EventCategory::Input)
        protected:
            KeyEvent(int keycode)
                : m_KeyCode(keycode) {}
            int m_KeyCode;
            
    };
    class KeyPressedEvent : public KeyEvent
    {
        public:
		KeyPressedEvent(const int keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		bool IsRepeat() const { return m_IsRepeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
        private:
            bool m_IsRepeat;
    };
    class KeyReleasedEvent : public KeyEvent
    {
        public:
            KeyReleasedEvent(int keycode)
                : KeyEvent(keycode) {}
            EVENT_CLASS_TYPE(KeyReleased)
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "KeyRealeasedEvent" << ": " << m_KeyCode;
                return ss.str();
            }
    };
    class KeyTypedEvent : public KeyEvent
    {
        public:
            KeyTypedEvent(int keycode)
                : KeyEvent(keycode) {}
            EVENT_CLASS_TYPE(KeyTyped)
            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "KeyTypedEvent" << ": " << m_KeyCode;
                return ss.str();
            }
    };
}
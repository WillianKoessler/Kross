#pragma once

#include "Event.h"

#include <sstream>

namespace Kross {
	class KAPI KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_nKeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		int m_nKeyCode = -1;
		int m_nRepeatCount = -1;

		KeyEvent() = default;
		KeyEvent(int keycode, int repeat = -1)
			: m_nKeyCode(keycode), m_nRepeatCount(repeat) {}
	};

	class KAPI KeyHeldEvent : public KeyEvent
	{
	public:
		KeyHeldEvent(int keycode, int repeatCount)
			: KeyEvent(keycode, repeatCount) {}

		const char *ToString() const override
		{
			std::stringstream ss;
			ss << "Event: KeyHeld-> " << m_nKeyCode << " (" << m_nRepeatCount << ") times";
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(KeyHeld)
	};

	class KAPI KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode)
			: KeyEvent(keycode) {}

		const char* ToString() const override
		{
			std::stringstream ss;
			ss << "Event: KeyPressed-> " << m_nKeyCode;
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class KAPI KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		const char *ToString() const override
		{
			std::stringstream ss;
			ss << "Event: KeyReleased-> " << m_nKeyCode;
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KAPI KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		const char* ToString() const override
		{
			std::stringstream ss;
			ss << "Event: KeyTyped-> " << m_nKeyCode;
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

	//class KAPI KeyTypedEvent : public KeyEvent
	//{
	//public:
	//	KeyTypedEvent(int keycode)
	//		: KeyEvent(keycode) {}

	//	std::string ToString() const override
	//	{
	//		std::stringstream ss;
	//		ss << "KeyTypedEvent: " << m_nKeyCode;
	//		return ss.str();
	//	}

	//	EVENT_CLASS_TYPE(KeyTyped)
	//};
}
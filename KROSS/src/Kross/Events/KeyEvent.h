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
		KeyEvent(){}
		KeyEvent(int keycode)
			: m_nKeyCode(keycode) {}

		int m_nKeyCode;
	};

	class KAPI KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_nRepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_nRepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Event: KeyPressed-> " << m_nKeyCode << " (" << m_nRepeatCount << ") times";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	protected:
		int m_nRepeatCount;
	};

	class KAPI KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString()
		{
			std::stringstream ss;
			ss << "Event: KeyReleased-> " << m_nKeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KAPI KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Event: KeyTyped-> " << m_nKeyCode;
			return ss.str();
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
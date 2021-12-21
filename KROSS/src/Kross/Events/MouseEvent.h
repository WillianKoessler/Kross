#pragma once

#include "Event.h"
#include "MouseButtonCodes.h"
#include <sstream>

namespace Kross {
	class KAPI MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int x, int y)
			: m_fMouseX((float)x), m_fMouseY((float)y) {}

		inline float GetX() const { return m_fMouseX; }
		inline float GetY() const { return m_fMouseY; }

		const char *ToString() const override
		{
			std::stringstream ss;
			ss << "Event: MouseMove -> (" << m_fMouseX << ", " << m_fMouseY << ")";
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_fMouseX, m_fMouseY;
	};

	class KAPI MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffSet, float yOffSet)
			: m_fXOffSet(xOffSet), m_fYOffSet(yOffSet) {}

		inline float GetXOffSet() const { return m_fXOffSet; }
		inline float GetYOffSet() const { return m_fYOffSet; }

		const char *ToString() const override
		{
			std::stringstream ss;
			ss << "Event: MouseScrolled -> (" << m_fXOffSet<< ", " << m_fYOffSet << ")";
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_fXOffSet, m_fYOffSet;
	};

	class KAPI MouseButtonEvent : public Event
	{
	public:
		inline MouseButton GetMouseButton() const { return (MouseButton)m_nButton; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		int m_nButton, m_nRepeat;
		MouseButtonEvent(int button, int repeat = -1)
			: m_nButton(button), m_nRepeat(repeat) {}
	};

	class KAPI MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		const char *ToString() const override
		{
			std::stringstream ss;
			ss << "Event: MousePressed -> " << m_nButton;
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(MousePressed)
	};

	class KAPI MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		const char *ToString() const override
		{
			std::stringstream ss;
			ss << "Event: MouseReleased -> " << m_nButton;
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(MouseReleased)
	};

	class KAPI MouseButtonHeldEvent : public MouseButtonEvent
	{
	public:
		MouseButtonHeldEvent(int button, int repeat)
			: MouseButtonEvent(button, repeat) {}

		const char *ToString() const override
		{
			std::stringstream ss;
			ss << "Event: MouseHeld -> " << m_nButton;
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(MouseHeld)
	};
}

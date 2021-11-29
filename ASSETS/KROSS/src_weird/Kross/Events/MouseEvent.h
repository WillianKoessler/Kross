#pragma once

#include "Event.h"

#include <sstream>

namespace Kross {
	class KAPI MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int x, int y)
			: m_fMouseX((float)x), m_fMouseY((float)y) {}

		inline float GetX() const { return m_fMouseX; }
		inline float GetY() const { return m_fMouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Event: MouseMove -> (" << m_fMouseX << ", " << m_fMouseY << ")";
			return ss.str();
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

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Event: MouseScrolled -> (" << m_fXOffSet<< ", " << m_fYOffSet << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_fXOffSet, m_fYOffSet;
	};

	class KAPI MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_nButton; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button)
			: m_nButton(button) {}
		int m_nButton;
	};

	class KAPI MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Event: MousePressed -> " << m_nButton;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MousePressed)
	};

	class KAPI MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Event: MouseReleased -> " << m_nButton;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseReleased)
	};
}
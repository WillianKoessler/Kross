#pragma once

#include "Event.h"

#include <sstream>

namespace Kross {
	class KAPI WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_unWidth(width), m_unHeight(height) {}

		inline unsigned int GetWidth()  const { return m_unWidth; }
		inline unsigned int GetHeight() const { return m_unHeight; }

		const char *ToString() const override
		{
			std::stringstream ss;
			ss << "Event: Window Resize -> " << m_unWidth << " x " << m_unHeight;
			return ss.str().c_str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_unWidth, m_unHeight;
	};

	class KAPI WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent(){}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class KAPI AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class KAPI AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class KAPI AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}
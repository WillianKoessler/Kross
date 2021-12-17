#pragma once
#include "Panel.h"

namespace Kross {
	class DockSpace : public Panel
	{
		bool m_bShowed = false;
		bool m_bMessageBox = false;
		bool m_bStyleEditor = false;
		bool m_bFullscreen = true;
		uint32_t m_DockFlags = 0;
		virtual void Menu() override;
	public:
		DockSpace();
		virtual void Show(double ts) override;
	};
}

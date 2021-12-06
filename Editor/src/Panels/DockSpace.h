#pragma once
#include "Panel.h"

namespace Kross {
	class DockSpace : public Panel
	{
		bool m_bOpen = true;
		bool m_bShowed = false;
		bool m_bMessageBox = false;
		bool m_bStyleEditor = false;
		bool m_bFullscreen = true;
	public:
		DockSpace(const char* name);
		virtual void Show(double ts) override;
	};
}

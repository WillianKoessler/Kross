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
		std::string m_strName;
	public:
		DockSpace(const std::string& name);
		virtual ~DockSpace() override;
		virtual void Show(double ts) override;
	};
}

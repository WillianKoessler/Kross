#pragma once

#include "Kross/Core/Core.h"
#include <filesystem>

namespace Kross {
	class ContentBrowser : public Resource
	{
	public:
		ContentBrowser();
		void Show();
		void Settings();
	private:
		uint32_t m_Flags = 0;
		int iconScaleMin = 64;
		int iconScaleMax = 512;
		int iconScale = 100;
		float doubleClickTiming = 1.0f;
		std::filesystem::path m_CurrentDirectory;
	};
}


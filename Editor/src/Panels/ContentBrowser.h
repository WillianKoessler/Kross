#pragma once

#include "Kross/Core/Core.h"
#include <filesystem>

namespace Kross {
	class ContentBrowser : public Resource
	{
	public:
		ContentBrowser();
		void Show();
	private:
		uint32_t m_Flags;
		std::filesystem::path m_CurrentDirectory;
		float iconScale;
	};
}


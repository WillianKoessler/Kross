#pragma once

#include "Kross/Core/Core.h"

namespace Kross {
	class ContentBrowser : public Resource
	{
	public:
		ContentBrowser();
		void Show(double ts);
	private:
		uint32_t m_Flags;
	};
}


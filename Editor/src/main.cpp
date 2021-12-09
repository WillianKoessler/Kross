#include "Editor_pch.h"
#ifdef TEST
#include <iostream>
#include <vector>
#include <algorithm>

static std::vector<int *> pool;

void func(int *val)
{
	auto location = [&](int *v) { return std::lower_bound(pool.begin(), pool.end(), v); };
	auto i = location(val);
	if (i != pool.end()) {
		if (*i) {
			if (*i == val) {
				std::cout << " '" << *(*i) << "' ";
			} else {
				pool.emplace(i, val);
			}
		} else pool.emplace(i, (int*)0);
	} else pool.emplace(i, (int *)0);
	if (*val == 2) delete val;
}

int main()
{
	int *arr[] = { new int(1), new int(3), new int(2), new int(1), new int(20), new int(10) };
	for (int i = 0; i < sizeof(arr) / sizeof(int *); i++) func(arr[i]);
	std::cout << std::endl;
	for (int *i : pool) std::cout << " '" << *i << "' ";
	return 0;
}

#else

#include "Kross/Core/EntryPoint.h"

#include "EditorLayer.h"
namespace Kross {
	class KrossEditor : public Application
	{
	public:
		KrossEditor()
			: Application("Kross Editor", 800, 400, Kross::Renderer::TWO_D)
		{
			PushLayer(makeRef<EditorLayer>());
		}
		~KrossEditor()
		{
		}
	};
}

Kross::Application *CreateApp()
{
	return new Kross::KrossEditor();
}

#endif
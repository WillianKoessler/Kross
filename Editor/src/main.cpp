#include "Editor_pch.h"
#ifdef TEST
#else
#include "Kross/Core/EntryPoint.h"

#include "EditorLayer.h"
namespace Kross {
	class KrossEditor : public Application
	{
	public:
		KrossEditor() : Application("Kross Editor", 800, 400, Kross::Renderer::TWO_D)
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
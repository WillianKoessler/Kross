#include "Kross.h"
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

	Application* CreateApp()
	{
		return new KrossEditor();
	}
}

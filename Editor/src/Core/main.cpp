#define KROSS_MAIN 1
#include "Kross.h"

#include "Core/world.h"
#include "Core/UI.h"

namespace Kross {
	class Game : public Application
	{
		Ref<UI> m_UI;
		Ref<world> m_World;

	public:
		Game()
			: Application("Kross Editor", 800, 400, Kross::Renderer::TWO_D)
		{
			m_World = makeRef<world>();
			m_UI = makeRef<UI>();
			PushLayer({ m_World, m_UI });
			//m_UI->setCanvas(m_World->)
		}
		~Game()
		{
		}
	};
}

Kross::Application* Kross::CreateApp()
{
	return new Game();
}

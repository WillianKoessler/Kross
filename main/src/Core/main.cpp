#define KROSS_MAIN 1
#include "Kross.h"

#include "World/Canvas.h"

class Game : public Kross::Application
{
public:
	Game()
		: Kross::Application("RPG", 800, 400)
	{
		GetWindow().SetVSync(true);
		PushLayer(Kross::makeRef<Canvas>());
	}
	~Game()
	{

	}
};


Kross::Application* Kross::CreateApp()
{
	return new Game();
}

#define KROSS_MAIN 1
#include "Kross.h"

#include "Core/world.h"

class Game : public Kross::Application
{
public:
	Game()
		: Kross::Application("Voxel", 800, 400, Kross::Renderer::D3)
	{
		PushLayer(Kross::makeRef<world>());
	}
	~Game()
	{}
};


Kross::Application* Kross::CreateApp()
{
	return new Game();
}

#define KROSS_MAIN 1
#include "Kross.h"

#include "world.h"

class Game : public Kross::Application
{
public:
	Game() : Kross::Application("Voxel", 800, 400, Kross::Renderer::TWO_D)
	{
		PushLayer(Kross::makeRef<world>());
	}
	~Game()
	{}
};


Kross::Application* Kross::CreateApp()
{
//	return nullptr;
	return new Game();
}

//int main(int argc, char** args)
//{
//	Kross::Logger::Init();
//	Game* app = new Game();
//	if (app) app->Run();
//	else KROSS_CORE_FATAL("Could not initialize Engine");
//	delete app;
//	return 0;
//}

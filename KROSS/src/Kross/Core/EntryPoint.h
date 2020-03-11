#pragma once
#include "Core.h"

#if KROSS_PLATFORM_WINDOWS
extern Kross::Application* Kross::CreateApp();

#if KROSS_MAIN
int main(int agrc, char** args)
{
	Kross::Log::Init();
	//Kross::Log::GetCoreLogger()->info("\ntest1 {0}\n test2 {1}", __FUNCTION__, __FUNCSIG__);

	KROSS_PROFILE_BEGIN("StartUp", "profiles/StartUp_Profile.json");
	Kross::Application* app = Kross::CreateApp();

	KROSS_PROFILE_OVERRIDE("Runtime", "profiles/RunTime_Profile.json");
	app->Run();

	KROSS_PROFILE_OVERRIDE("Shutdown", "profiles/Shutdown_Profile.json");
	delete app;
	KROSS_PROFILE_END;
}
#endif

#else
#error Kross only supports Windows... for now
#endif
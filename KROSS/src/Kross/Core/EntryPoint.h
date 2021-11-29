#pragma once
#include "Core.h"

#if KROSS_PLATFORM_WINDOWS
extern Kross::Application* Kross::CreateApp();
#else
#error Kross only supports Windows... for now
#endif

#if KROSS_MAIN
int main(int agrc, char** args)
{
	Kross::Logger::Init();
	Kross::Application* app;

	KROSS_PROFILE_BEGIN("StartUp", "profiles/StartUp_Profile.json");
	try
	{
		 app = Kross::CreateApp();
	}
	catch (const std::exception& e)
	{
		KROSS_CORE_FATAL("Unhandled exception at App Creation: {0}", e.what());
	}

	KROSS_PROFILE_OVERRIDE("Runtime", "profiles/RunTime_Profile.json");
	try
	{
		app->Run();
	}
	catch (const std::exception & e)
	{
		KROSS_CORE_FATAL("Unhandled exception at Runtime: {0}", e.what());
	}

	KROSS_PROFILE_OVERRIDE("Shutdown", "profiles/Shutdown_Profile.json");
	try
	{
		delete app;
	}
	catch (const std::exception & e)
	{
		KROSS_CORE_FATAL("Unhandled exception at App Destruction : {0}", e.what());
	}

	KROSS_PROFILE_END;
}
#endif

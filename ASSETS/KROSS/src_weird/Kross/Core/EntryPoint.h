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
	Kross::Log::Init();
	Kross::Application* app;

	KROSS_PROFILE_BEGIN("StartUp", "profiles/StartUp_Profile.json");
	try
	{
		 app = Kross::CreateApp();
	}
	catch (const std::exception& e)
	{
		KROSS_MSGBOX(std::string("Unhandled exception at App Creation: ") + e.what(), __FUNCSIG__, _FATAL_);
	}

	KROSS_PROFILE_OVERRIDE("Runtime", "profiles/RunTime_Profile.json");
	try
	{
		app->Run();
	}
	catch (const std::exception & e)
	{
		KROSS_MSGBOX(std::string("Unhandled exception at Runtime: ") + e.what(), __FUNCSIG__, _FATAL_);
	}

	KROSS_PROFILE_OVERRIDE("Shutdown", "profiles/Shutdown_Profile.json");
	try
	{
		delete app;
	}
	catch (const std::exception & e)
	{
		KROSS_MSGBOX(std::string("Unhandled exception at App Destruction: ") + e.what(), __FUNCSIG__, _FATAL_);
	}

	KROSS_PROFILE_END;
}
#endif

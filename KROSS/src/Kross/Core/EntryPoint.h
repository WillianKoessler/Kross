#pragma once
#include "Core.h"
#include "Application.h"

#if KROSS_PLATFORM_WINDOWS
extern Kross::Application* Kross::CreateApp();
#else
#error Kross only supports Windows... for now
#endif

int main(int agrc, char** args)
{
	Kross::Logger::Init();
	SPDLOG_LOGGER_CALL(Kross::Logger::Core(), spdlog::level::critical, "Welcome to Kross Engine");
	SPDLOG_LOGGER_CALL(Kross::Logger::Core(), spdlog::level::info, "Logger Initialized");
	Kross::Application* app;

	KROSS_PROFILE_BEGIN("StartUp", "profiles/StartUp_Profile.json");
	try { app = Kross::CreateApp(); } catch (const std::exception& e) { KROSS_FATAL("Unhandled exception at App Creation: {0}", e.what()); }

	KROSS_PROFILE_OVERRIDE("Runtime", "profiles/RunTime_Profile.json");
	try { if(app) app->Run(); } catch (const std::exception & e) { KROSS_FATAL("Unhandled exception at Runtime: {0}", e.what()); }

	KROSS_PROFILE_OVERRIDE("Shutdown", "profiles/Shutdown_Profile.json");
	try { if(app) delete app; } catch (const std::exception & e) { KROSS_FATAL("Unhandled exception at App Destruction : {0}", e.what()); }

	KROSS_PROFILE_END;
}

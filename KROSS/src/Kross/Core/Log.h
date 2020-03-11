#pragma once

#include "Core.h"

namespace spdlog {
	class logger;
}

namespace Kross {
	class KAPI Log
	{
	public:
		static void Init();
		static void WindowBox(const std::string& message, const std::string& mode, unsigned int type);
		inline static Ref<spdlog::logger>& GetFileLogger() { return s_FileLogger; };
		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; };
		inline static Ref<spdlog::logger>& GetClientFileLogger() { return s_ClientFileLogger; };
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; };
		//inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; };
	private:
		static Ref<spdlog::logger> s_FileLogger;
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientFileLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

#define KROSS_MSGBOX(message, mode, type) ::Kross::Log::WindowBox(message, mode, type)

#define KROSS_R_MSGBOX(condition, code, message, type)\
if (condition)\
{\
code\
}\
else\
{\
	static bool called = false;\
	if (!called)\
	{\
		KROSS_MSGBOX(message, __FUNCTION__, type);\
		called = true;\
	}\
}

#undef KROSS_FILE_LOG
#define KROSS_FILE_LOG 0
#if KROSS_FILE_LOG
	// Core File Log Macros
	#define KROSS_CORE_FILE_TRACE(...)	::Kross::Log::GetFileLogger()->trace(__VA_ARGS__)
	#define KROSS_CORE_FILE_INFO(...)	::Kross::Log::GetFileLogger()->info(__VA_ARGS__)
	#define KROSS_CORE_FILE_WARN(...)	::Kross::Log::GetFileLogger()->warn(__VA_ARGS__)
	#define KROSS_CORE_FILE_ERROR(...)	::Kross::Log::GetFileLogger()->error(__VA_ARGS__)
	#define KROSS_CORE_FILE_FATAL(...)	::Kross::Log::GetFileLogger()->critical(__VA_ARGS__)
	#define KROSS_CORE_ERROR_(...)		::Kross::Log::GetFileLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
										::Kross::Log::GetFileLogger()->error(__VA_ARGS__)

	#define KROSS_CORE_TRACE(...)		::Kross::Log::GetCoreLogger()->trace(__VA_ARGS__);	::Kross::Log::GetFileLogger()->trace(__VA_ARGS__)
	#define KROSS_CORE_INFO(...)		::Kross::Log::GetCoreLogger()->info(__VA_ARGS__);	::Kross::Log::GetFileLogger()->info(__VA_ARGS__)
	#define KROSS_CORE_WARN(...)		::Kross::Log::GetCoreLogger()->warn(__VA_ARGS__);	::Kross::Log::GetFileLogger()->warn(__VA_ARGS__)
	#define KROSS_CORE_ERROR(...)		::Kross::Log::GetCoreLogger()->error(__VA_ARGS__);	::Kross::Log::GetFileLogger()->error(__VA_ARGS__)
	#define KROSS_CORE_FATAL(...)		::Kross::Log::GetCoreLogger()->critical(__VA_ARGS__);::Kross::Log::GetFileLogger()->critical(__VA_ARGS__)
	#define KROSS_CORE_ERROR_(...)		::Kross::Log::GetCoreLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
										::Kross::Log::GetCoreLogger()->error(__VA_ARGS__);\
										::Kross::Log::GetFileLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
										::Kross::Log::GetFileLogger()->error(__VA_ARGS__)

	#define KROSS_CORE_TRACE_ONCE(...) { static bool called = false; if(!called) {	Kross::Log::GetCoreLogger()->trace(__VA_ARGS__);	::Kross::Log::GetFileLogger()->trace(__VA_ARGS__);		called = true;}}
	#define KROSS_CORE_INFO_ONCE(...)  { static bool called = false; if(!called) {	Kross::Log::GetCoreLogger()->info(__VA_ARGS__);		::Kross::Log::GetFileLogger()->info(__VA_ARGS__);		called = true;}}
	#define KROSS_CORE_WARN_ONCE(...)  { static bool called = false; if(!called) {	Kross::Log::GetCoreLogger()->warn(__VA_ARGS__);		::Kross::Log::GetFileLogger()->warn(__VA_ARGS__);		called = true;}}
	#define KROSS_CORE_ERROR_ONCE_(...){ static bool called = false; if(!called) {	Kross::Log::GetCoreLogger()->error(__VA_ARGS__);	::Kross::Log::GetFileLogger()->error(__VA_ARGS__);		called = true;}}
	#define KROSS_CORE_FATAL_ONCE(...) { static bool called = false; if(!called) {	Kross::Log::GetCoreLogger()->critical(__VA_ARGS__);	::Kross::Log::GetFileLogger()->critical(__VA_ARGS__);	called = true;}}
	#define KROSS_CORE_ERROR_ONCE(...) { static bool called = false; if(!called) {	Kross::Log::GetCoreLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
																					Kross::Log::GetCoreLogger()->error(__VA_ARGS__);\
																					Kross::Log::GetFileLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
																					Kross::Log::GetFileLogger()->error(__VA_ARGS__); called = true;}


	// Client File Log Macros
	#define KROSS_FILE_TRACE(...)		::Kross::Log::GetClientFileLogger()->trace(__VA_ARGS__)
	#define KROSS_FILE_INFO(...)		::Kross::Log::GetClientFileLogger()->info(__VA_ARGS__)
	#define KROSS_FILE_WARN(...)		::Kross::Log::GetClientFileLogger()->warn(__VA_ARGS__)
	#define KROSS_FILE_ERROR(...)		::Kross::Log::GetClientFileLogger()->error(__VA_ARGS__)
	#define KROSS_FILE_FATAL(...)		::Kross::Log::GetClientFileLogger()->critical(__VA_ARGS__)

	#define KROSS_TRACE(...)			::Kross::Log::GetClientLogger()->trace(__VA_ARGS__);	Kross::Log::GetClientFileLogger()->trace(__VA_ARGS__)
	#define KROSS_INFO(...)				::Kross::Log::GetClientLogger()->info(__VA_ARGS__);		Kross::Log::GetClientFileLogger()->info(__VA_ARGS__)
	#define KROSS_WARN(...)				::Kross::Log::GetClientLogger()->warn(__VA_ARGS__);		Kross::Log::GetClientFileLogger()->warn(__VA_ARGS__)
	#define KROSS_ERROR_(...)			::Kross::Log::GetClientLogger()->error(__VA_ARGS__);	Kross::Log::GetClientFileLogger()->error(__VA_ARGS__)
	#define KROSS_FATAL(...)			::Kross::Log::GetClientLogger()->critical(__VA_ARGS__);	Kross::Log::GetClientFileLogger()->critical(__VA_ARGS__)
	#define KROSS_ERROR(...)			::Kross::Log::GetClientLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
										::Kross::Log::GetClientLogger()->error(__VA_ARGS__);\
										::Kross::Log::GetClientFileLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
										::Kross::Log::GetClientFileLogger()->error(__VA_ARGS__)

	#define KROSS_TRACE_ONCE(...) { static bool called = false; if(!called) {	Kross::Log::GetClientLogger()->trace(__VA_ARGS__);		Kross::Log::GetClientFileLogger()->trace(__VA_ARGS__);		called = true;}}
	#define KROSS_INFO_ONCE(...)  { static bool called = false; if(!called) {	Kross::Log::GetClientLogger()->info(__VA_ARGS__);		Kross::Log::GetClientFileLogger()->info(__VA_ARGS__);		called = true;}}
	#define KROSS_WARN_ONCE(...)  { static bool called = false; if(!called) {	Kross::Log::GetClientLogger()->warn(__VA_ARGS__);		Kross::Log::GetClientFileLogger()->warn(__VA_ARGS__);		called = true;}}
	#define KROSS_ERROR_ONCE_(...){ static bool called = false; if(!called) {	Kross::Log::GetClientLogger()->error(__VA_ARGS__);		Kross::Log::GetClientFileLogger()->error(__VA_ARGS__);		called = true;}}
	#define KROSS_FATAL_ONCE(...) { static bool called = false; if(!called) {	Kross::Log::GetClientLogger()->critical(__VA_ARGS__);	Kross::Log::GetClientFileLogger()->critical(__VA_ARGS__);	called = true;}}
	#define KROSS_ERROR_ONCE(...) { static bool called = false; if(!called) {	Kross::Log::GetClientLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
																				Kross::Log::GetClientLogger()->error(__VA_ARGS__);\
																				Kross::Log::GetClientFileLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
																				Kross::Log::GetClientFileLogger()->error(__VA_ARGS__); called = true;}

#else
	#define KROSS_CORE_FILE_TRACE(...)
	#define KROSS_CORE_FILE_INFO(...)
	#define KROSS_CORE_FILE_WARN(...)
	#define KROSS_CORE_FILE_ERROR(...)
	#define KROSS_CORE_FILE_FATAL(...)

	#define KROSS_CORE_TRACE(...) ::Kross::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define KROSS_CORE_INFO(...)  ::Kross::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define KROSS_CORE_WARN(...)  ::Kross::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define KROSS_CORE_ERROR_(...)::Kross::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define KROSS_CORE_FATAL(...) ::Kross::Log::GetCoreLogger()->critical(__VA_ARGS__)
	#define KROSS_CORE_ERROR(...) ::Kross::Log::GetCoreLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
								  ::Kross::Log::GetCoreLogger()->error(__VA_ARGS__)

	#define KROSS_CORE_TRACE_ONCE(...) { static bool called = false; if(!called) { Kross::Log::GetCoreLogger()->trace(__VA_ARGS__); called = true;}}
	#define KROSS_CORE_INFO_ONCE(...)  { static bool called = false; if(!called) { Kross::Log::GetCoreLogger()->info(__VA_ARGS__); called = true;}}
	#define KROSS_CORE_WARN_ONCE(...)  { static bool called = false; if(!called) { Kross::Log::GetCoreLogger()->warn(__VA_ARGS__); called = true;}}
	#define KROSS_CORE_ERROR_ONCE_(...){ static bool called = false; if(!called) { Kross::Log::GetCoreLogger()->error(__VA_ARGS__); called = true;}}
	#define KROSS_CORE_FATAL_ONCE(...) { static bool called = false; if(!called) { Kross::Log::GetCoreLogger()->critical(__VA_ARGS__); called = true;}}
	#define KROSS_CORE_ERROR_ONCE(...) { static bool called = false; if(!called) { Kross::Log::GetCoreLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
																				   Kross::Log::GetCoreLogger()->error(__VA_ARGS__); called = true;}

// Client File Log 
	#define KROSS_FILE_TRACE(...)
	#define KROSS_FILE_INFO(...)
	#define KROSS_FILE_WARN(...)
	#define KROSS_FILE_ERROR(...)
	#define KROSS_FILE_FATAL(...)

	#define KROSS_TRACE(...) ::Kross::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define KROSS_INFO(...)	 ::Kross::Log::GetClientLogger()->info(__VA_ARGS__)
	#define KROSS_WARN(...)	 ::Kross::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define KROSS_ERROR_(...)::Kross::Log::GetClientLogger()->error(__VA_ARGS__)
	#define KROSS_FATAL(...) ::Kross::Log::GetClientLogger()->critical(__VA_ARGS__)
	#define KROSS_ERROR(...) ::Kross::Log::GetClientLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
							 ::Kross::Log::GetClientLogger()->error(__VA_ARGS__)

	#define KROSS_TRACE_ONCE(...) { static bool called = false; if(!called) { Kross::Log::GetClientLogger()->trace(__VA_ARGS__); called = true;}}
	#define KROSS_INFO_ONCE(...)  { static bool called = false; if(!called) { Kross::Log::GetClientLogger()->info(__VA_ARGS__); called = true;}}
	#define KROSS_WARN_ONCE(...)  { static bool called = false; if(!called) { Kross::Log::GetClientLogger()->warn(__VA_ARGS__); called = true;}}
	#define KROSS_ERROR_ONCE_(...){ static bool called = false; if(!called) { Kross::Log::GetClientLogger()->error(__VA_ARGS__); called = true;}}
	#define KROSS_FATAL_ONCE(...) { static bool called = false; if(!called) { Kross::Log::GetClientLogger()->critical(__VA_ARGS__); called = true;}}
	#define KROSS_ERROR_ONCE(...) { static bool called = false; if(!called) { Kross::Log::GetClientLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
																				   Kross::Log::GetClientLogger()->error(__VA_ARGS__); called = true;}

#endif
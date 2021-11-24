#pragma once

#include "Core.h"

namespace spdlog {
	class logger;
}

namespace Kross {
	class KAPI ClientLog
	{
	public:
		static void Log(const char* message, uint8_t level, bool file);
	};
	class Log
	{
	public:
		static void Init();
		static void WindowBox(const std::string& message, const std::string& mode, unsigned int type);
		inline static Ref<spdlog::logger>& GetFileLogger() { return s_FileLogger; };
		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; };
		inline static Ref<spdlog::logger>& GetClientFileLogger() { return s_ClientFileLogger; };
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; };
	private:
		static Ref<spdlog::logger> s_FileLogger;
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientFileLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

#define KROSS_MSGBOX(message, mode, type) ::Kross::Log::WindowBox(message, mode, type)

#if(KROSS_FILE_LOG == true)
#ifdef KROSS_BUILD
	#define KROSS_CORE_FILE_TRACE(...)	::Kross::Log::GetFileLogger()->trace(__VA_ARGS__)
	#define KROSS_CORE_FILE_INFO(...)	::Kross::Log::GetFileLogger()->info(__VA_ARGS__)
	#define KROSS_CORE_FILE_WARN(...)	::Kross::Log::GetFileLogger()->warn(__VA_ARGS__)
	#define KROSS_CORE_FILE_ERROR(...)	::Kross::Log::GetFileLogger()->error(__VA_ARGS__)
	#define KROSS_CORE_FILE_FATAL(...)	::Kross::Log::GetFileLogger()->critical(__VA_ARGS__)
	#define KROSS_CORE_FILE_ERROR_(...)	::Kross::Log::GetFileLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
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

	#define KROSS_CORE_TRACE_ONCE(...)	{ static bool called = false; if(!called) {	Kross::Log::GetCoreLogger()->trace(__VA_ARGS__);	::Kross::Log::GetFileLogger()->trace(__VA_ARGS__);		called = true;}}
	#define KROSS_CORE_INFO_ONCE(...)	{ static bool called = false; if(!called) {	Kross::Log::GetCoreLogger()->info(__VA_ARGS__);		::Kross::Log::GetFileLogger()->info(__VA_ARGS__);		called = true;}}
	#define KROSS_CORE_WARN_ONCE(...)	{ static bool called = false; if(!called) {	Kross::Log::GetCoreLogger()->warn(__VA_ARGS__);		::Kross::Log::GetFileLogger()->warn(__VA_ARGS__);		called = true;}}
	#define KROSS_CORE_ERROR_ONCE(...)	{ static bool called = false; if(!called) {	Kross::Log::GetCoreLogger()->error(__VA_ARGS__);	::Kross::Log::GetFileLogger()->error(__VA_ARGS__);		called = true;}}
	#define KROSS_CORE_FATAL_ONCE(...)	{ static bool called = false; if(!called) {	Kross::Log::GetCoreLogger()->critical(__VA_ARGS__);	::Kross::Log::GetFileLogger()->critical(__VA_ARGS__);	called = true;}}
	#define KROSS_CORE_ERROR_ONCE_(...)	{ static bool called = false; if(!called) {	Kross::Log::GetCoreLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
																					Kross::Log::GetCoreLogger()->error(__VA_ARGS__);\
																					Kross::Log::GetFileLogger()->error("ERROR AT FILE {0}, LINE {1}",__FILE__, __LINE__);\
																					Kross::Log::GetFileLogger()->error(__VA_ARGS__); called = true;}
#else //KROSS_BUILD
	#define KROSS_CORE_FILE_TRACE(...)
	#define KROSS_CORE_FILE_INFO(...)
	#define KROSS_CORE_FILE_WARN(...)
	#define KROSS_CORE_FILE_ERROR(...)
	#define KROSS_CORE_FILE_FATAL(...)
	#define KROSS_CORE_ERROR_(...)		

	#define KROSS_CORE_TRACE(...)		
	#define KROSS_CORE_INFO(...)		
	#define KROSS_CORE_WARN(...)		
	#define KROSS_CORE_ERROR(...)		
	#define KROSS_CORE_FATAL(...)		
	#define KROSS_CORE_ERROR_(...)		

	#define KROSS_CORE_TRACE_ONCE(...)
	#define KROSS_CORE_INFO_ONCE(...) 
	#define KROSS_CORE_WARN_ONCE(...) 
	#define KROSS_CORE_ERROR_ONCE_(...)
	#define KROSS_CORE_FATAL_ONCE(...)
	#define KROSS_CORE_ERROR_ONCE(...)
#endif //KROSS_BUILD

	#define KROSS_FILE_TRACE(msg)	::Kross::ClientLog::Log(msg, 0, true)
	#define KROSS_FILE_INFO(msg)	::Kross::ClientLog::Log(msg, 1, true)
	#define KROSS_FILE_WARN(msg)	::Kross::ClientLog::Log(msg, 2, true)
	#define KROSS_FILE_ERROR(msg)	::Kross::ClientLog::Log(msg, 3, true)
	#define KROSS_FILE_FATAL(msg)	::Kross::ClientLog::Log(msg, 4, true)

	#define KROSS_TRACE(msg)	::Kross::ClientLog::Log(msg, 0, false)
	#define KROSS_INFO(msg)		::Kross::ClientLog::Log(msg, 1, false)
	#define KROSS_WARN(msg)		::Kross::ClientLog::Log(msg, 2, false)
	#define KROSS_ERROR(msg)	::Kross::ClientLog::Log(msg, 3, false)
	#define KROSS_FATAL(msg)	::Kross::ClientLog::Log(msg, 4, false)

#else //KROSS_FILE_LOG
#ifdef KROSS_BUILD
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
																				   Kross::Log::GetCoreLogger()->error(__VA_ARGS__); called = true;}}
#else //KROSS_BUILD
	#define KROSS_CORE_FILE_TRACE(...)
	#define KROSS_CORE_FILE_INFO(...)
	#define KROSS_CORE_FILE_WARN(...)
	#define KROSS_CORE_FILE_ERROR(...)
	#define KROSS_CORE_FILE_FATAL(...)

	#define KROSS_CORE_TRACE(...) 
	#define KROSS_CORE_INFO(...)  
	#define KROSS_CORE_WARN(...)  
	#define KROSS_CORE_ERROR_(...)
	#define KROSS_CORE_FATAL(...) 
	#define KROSS_CORE_ERROR(...) 
									

	#define KROSS_CORE_TRACE_ONCE(...) 
	#define KROSS_CORE_INFO_ONCE(...)  
	#define KROSS_CORE_WARN_ONCE(...)  
	#define KROSS_CORE_ERROR_ONCE_(...)
	#define KROSS_CORE_FATAL_ONCE(...) 
	#define KROSS_CORE_ERROR_ONCE(...) 
#endif //KROSS_BUILD

	#define KROSS_FILE_TRACE(msg)
	#define KROSS_FILE_INFO(msg)
	#define KROSS_FILE_WARN(msg)
	#define KROSS_FILE_ERROR(msg)
	#define KROSS_FILE_FATAL(msg)

	#define KROSS_TRACE(msg)	::Kross::ClientLog::Log(msg, 0, false)
	#define KROSS_INFO(msg)		::Kross::ClientLog::Log(msg, 1, false)
	#define KROSS_WARN(msg)		::Kross::ClientLog::Log(msg, 2, false)
	#define KROSS_ERROR(msg)	::Kross::ClientLog::Log(msg, 3, false)
	#define KROSS_FATAL(msg)	::Kross::ClientLog::Log(msg, 4, false)
#endif // KROSS_FILE_LOG
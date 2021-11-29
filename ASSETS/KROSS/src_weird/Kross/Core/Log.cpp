#include "Kross_pch.h"
#include "Log.h"
//#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/include/spdlog/sinks/basic_file_sink.h"

namespace Kross {
	Ref<spdlog::logger> Log::s_FileLogger;
	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientFileLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		try
		{
			std::vector<spdlog::sink_ptr> logSinks(2);
			logSinks[0] = (makeRef<spdlog::sinks::stdout_color_sink_mt>());
			
			logSinks[0]->set_pattern("%^[%T] %n: %v%$");
			logSinks[1] = (makeRef<spdlog::sinks::basic_file_sink_mt>("logs/Kross.log", true));
			logSinks[1]->set_pattern("[%T] [%l] %n: %v");

			s_CoreLogger = makeRef<spdlog::logger>("KROSS", logSinks.begin(), logSinks.end());
			spdlog::register_logger(s_CoreLogger);
			s_CoreLogger->set_level(spdlog::level::trace);
			s_CoreLogger->flush_on(spdlog::level::trace);

			logSinks[1] = makeRef<spdlog::sinks::basic_file_sink_mt>("logs/Client.log", true);
			logSinks[1]->set_pattern("[%T] [%l] %n: %v");

			s_ClientLogger = makeRef<spdlog::logger>("APP", logSinks.begin(), logSinks.end());
			spdlog::register_logger(s_ClientLogger);
			s_ClientLogger->set_level(spdlog::level::trace);
			s_ClientLogger->flush_on(spdlog::level::trace);



			//spdlog::set_pattern("%^[%T] %n %v%$");

			//s_CoreLogger = spdlog::stdout_color_mt("KROSS");

			//s_FileLogger = spdlog::basic_logger_mt("ENGINE", "logs/core_engine_log.txt", true);
			//s_FileLogger->set_level(spdlog::level::trace);

			//s_ClientLogger = spdlog::stdout_color_mt("APP");
			//s_ClientLogger->set_level(spdlog::level::trace);

			//s_ClientFileLogger = spdlog::basic_logger_mt("CLIENT", "logs/client_application_log.txt", true);
			//s_ClientFileLogger->set_level(spdlog::level::trace);

			KROSS_CORE_FATAL("Welcome to Kross Engine");
			KROSS_CORE_INFO("Initialized Log");
		}
		catch (const spdlog::spdlog_ex & ex)
		{
			std::cout << "Log init failed: " << ex.what() << std::endl;
		}
	}

	void Log::WindowBox(const std::string& m, const std::string& s, unsigned int type)
	{

#if UNICODE
		std::wstring& message = std::wstring(m.begin(), m.end());
		std::wstring& source = std::wstring(s.begin(), s.end());
		#define MB_MESSAGETITLE(mod, cod) (std::wstring(source+L" ["#mod"] Code: ") + std::to_wstring(cod)).c_str()
#else
		std::string& message = m;
		std::string& source = s;
		#define MB_MESSAGETITLE(mod, cod) (std::string(source+" ["#mod"] Code: ") + std::to_string(cod)).c_str()
#endif
		
		switch (type)
		{
		case _INFO_:
			s_CoreLogger->info("{0} MESSAGE: {1}", s, m);
			MessageBox(ACTIVEWINDOW, message.c_str(), MB_MESSAGETITLE("INFO", type), MB_ICONEXCLAMATION);
			break;
		case _WARN_:
			s_CoreLogger->warn("{0} WARNING: {1}", s, m);
			MessageBox(ACTIVEWINDOW, message.c_str(), MB_MESSAGETITLE("WARN", type), MB_ICONEXCLAMATION);
			break;
		case _ERROR_:
			s_CoreLogger->error("{0} ERROR: {1}", s, m);
			MessageBox(ACTIVEWINDOW, message.c_str(), MB_MESSAGETITLE("ERROR", type), MB_ICONERROR);
			MessageBeep(1000);
			break;
		case _FATAL_:
			MessageBox(ACTIVEWINDOW, message.c_str(), MB_MESSAGETITLE("FATAL", type), MB_ICONERROR);
			exit(type);
			break;
		default:
			s_CoreLogger->trace("{0} {1}", s, m);
			break;
		}
	}

}


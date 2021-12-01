#include <Kross_pch.h>
#include "Kross/Core/CoreLog.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/ostream_sink.h"

namespace Kross {
	KAPI Ref<spdlog::logger> Logger::s_FileLogger;
	KAPI Ref<spdlog::logger> Logger::s_CoreLogger;
	KAPI Ref<spdlog::logger> Logger::s_ClientFileLogger;
	KAPI Ref<spdlog::logger> Logger::s_ClientLogger;

	void Logger::Init()
	{
		try
		{
			std::vector<spdlog::sink_ptr> sinks;
			sinks.emplace_back(makeRef<spdlog::sinks::stdout_color_sink_mt>());
			sinks.emplace_back(makeRef<spdlog::sinks::basic_file_sink_mt>("Kross.log", true));
			sinks.emplace_back(makeRef<spdlog::sinks::basic_file_sink_mt>("Client.log", true));

			sinks[0]->set_pattern("%^[%T] %n: %v%$");
			sinks[1]->set_pattern("[%T] [%l] %n: %v");
			sinks[2]->set_pattern("[%T] [%l] %n: %v");

			{
				s_CoreLogger = makeRef<spdlog::logger>("Kross", begin(sinks), end(sinks));
				//spdlog::register_logger(std::make_shared<spdlog::logger>(s_CoreLogger));
				spdlog::register_logger(s_CoreLogger);
				s_CoreLogger->set_level(spdlog::level::trace);
				s_CoreLogger->flush_on(spdlog::level::trace);
			}{
				s_ClientLogger = makeRef<spdlog::logger>("APP", begin(sinks), end(sinks));
				//spdlog::register_logger(std::make_shared<spdlog::logger>(s_ClientLogger));
				spdlog::register_logger(s_ClientLogger);
				s_ClientLogger->set_level(spdlog::level::trace);
				s_ClientLogger->flush_on(spdlog::level::trace);
			}
			s_CoreLogger->critical("Welcome to Kross Engine");
			s_CoreLogger->info("Initialized Logger");
		} catch (const spdlog::spdlog_ex& ex)
		{
			std::cout << "Logger init failed: " << ex.what() << std::endl;
		}
	}
}


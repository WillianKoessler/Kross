#include <Kross_pch.h>
#include "Kross/Core/CoreLog.h"

#include "spdlog/include/spdlog/fmt/ostr.h"
#include "spdlog/include/spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/ostream_sink.h"

namespace Kross {
	KAPI Ref<spdlog::logger> Logger::s_CoreLogger;
	KAPI Ref<spdlog::logger> Logger::s_ClientLogger;

	void Logger::Init()
	{
		try
		{
			auto console = makeRef<spdlog::sinks::stdout_color_sink_mt>();
			auto coreFile = makeRef<spdlog::sinks::basic_file_sink_mt>("logs/Kross.log", true);
			auto clientFile = makeRef<spdlog::sinks::basic_file_sink_mt>("logs/Client.log", true);

			console->set_pattern("%^[%T] %n: [%!] | %v%$");
			coreFile->set_pattern("[%x - %T] [%n %l]: \t[%!] | %v");
			clientFile->set_pattern("[%x - %T] [%n %l]: \t[%!] | %v");

			std::vector<spdlog::sink_ptr> coreSinks;
			coreSinks.emplace_back(console);
			coreSinks.emplace_back(coreFile);

			std::vector<spdlog::sink_ptr> clientSinks;
			clientSinks.emplace_back(console);
			clientSinks.emplace_back(coreFile);
			clientSinks.emplace_back(clientFile);

			s_CoreLogger = makeRef<spdlog::logger>("KROSS", begin(coreSinks), end(coreSinks));
			spdlog::register_logger(s_CoreLogger);
			s_CoreLogger->set_level(spdlog::level::trace);
			s_CoreLogger->flush_on(spdlog::level::trace);

			s_ClientLogger = makeRef<spdlog::logger>("CLIENT", begin(clientSinks), end(clientSinks));
			spdlog::register_logger(s_ClientLogger);
			s_ClientLogger->set_level(spdlog::level::trace);
			s_ClientLogger->flush_on(spdlog::level::trace);
		} catch (const spdlog::spdlog_ex& ex)
		{
			std::cout << "Logger init failed: " << ex.what() << std::endl;
		}
	}
}


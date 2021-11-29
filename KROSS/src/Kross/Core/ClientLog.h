#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/CoreLog.h"

//namespace Kross {
//	class KAPI Log
//	{
//	public:
//		static constexpr uint8_t Console = 1;
//		static constexpr uint8_t File = 2;
//		static constexpr uint8_t MsgBox = 3;
//		static constexpr uint8_t trace = 1;
//		static constexpr uint8_t info = 2;
//		static constexpr uint8_t warn = 3;
//		static constexpr uint8_t error = 4;
//		static constexpr uint8_t critical = 5;
//
//		template<typename...Args>
//		static void to(uint8_t t, uint8_t s, const char* srcFunc, const char* fmt, const Args&...args)
//		{
//			Logger::Client(t, s, fmt, args...);
//		}
//	};
//}

#if(KROSS_FILE_LOG == true)
#define KROSS_FILE_TRACE(fmt, ...) ::Kross::Logger::Client(Kross::Logger::File, Kross::Logger::trace, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_FILE_INFO(fmt, ...)  ::Kross::Logger::Client(Kross::Logger::File, Kross::Logger::info, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_FILE_WARN(fmt, ...)  ::Kross::Logger::Client(Kross::Logger::File, Kross::Logger::warn, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_FILE_ERROR(fmt, ...) ::Kross::Logger::Client(Kross::Logger::File, Kross::Logger::error, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_FILE_FATAL(fmt, ...) ::Kross::Logger::Client(Kross::Logger::MsgBox, Kross::Logger::critical, __FUNCTION__, fmt, __VA_ARGS__)
#else //KROSS_FILE_LOG
#define KROSS_FILE_TRACE(fmt, ...)
#define KROSS_FILE_INFO(fmt, ...)
#define KROSS_FILE_WARN(fmt, ...)
#define KROSS_FILE_ERROR(fmt, ...)
#define KROSS_FILE_FATAL(fmt, ...)
#endif //KROSS_FILE_LOG

#define KROSS_TRACE(fmt, ...) ::Kross::Logger::Client(Kross::Logger::Console, Kross::Logger::trace, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_INFO(fmt, ...)  ::Kross::Logger::Client(Kross::Logger::Console, Kross::Logger::info, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_WARN(fmt, ...)  ::Kross::Logger::Client(Kross::Logger::Console, Kross::Logger::warn, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_ERROR(fmt, ...) ::Kross::Logger::Client(Kross::Logger::Console, Kross::Logger::error, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_FATAL(fmt, ...) ::Kross::Logger::Client(Kross::Logger::MsgBox, Kross::Logger::critical, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_TRACE_ONCE(fmt, ...) { static bool called = false; if(!called) {	::Kross::Logger::Client(Kross::Logger::File, Kross::Logger::trace, __FUNCTION__, fmt, __VA_ARGS__)called = true;}}
#define KROSS_INFO_ONCE(fmt, ...)  { static bool called = false; if(!called) {	::Kross::Logger::Client(Kross::Logger::File, Kross::Logger::info, __FUNCTION__, fmt, __VA_ARGS__)called = true;}}
#define KROSS_WARN_ONCE(fmt, ...)  { static bool called = false; if(!called) {	::Kross::Logger::Client(Kross::Logger::File, Kross::Logger::warn, __FUNCTION__, fmt, __VA_ARGS__)called = true;}}
#define KROSS_ERROR_ONCE(fmt, ...) { static bool called = false; if(!called) {	::Kross::Logger::Client(Kross::Logger::File, Kross::Logger::error, __FUNCTION__, fmt, __VA_ARGS__)called = true;}}

#pragma once


#ifndef CORE_CONFIG
#define CORE_CONFIG


/*
	Configuration File for Core.h
*/

#define GraphicsAPI OpenGL

#ifndef KROSS_DEBUG
#define KROSS_DEBUG 1
#endif
#ifndef KROSS_GL_DEBUG
#define KROSS_GL_DEBUG 1
#endif
#ifndef KROSS_GLCALL_PROFILE
#define KROSS_GLCALL_PROFILE 0
#endif
#ifndef KROSS_RENDERER_PROFILE
#define KROSS_RENDERER_PROFILE 0
#endif
#ifndef KROSS_ENABLE_ASSERTS
#define KROSS_ENABLE_ASSERTS 0
#endif
#ifndef KROSS_FILE_LOG
#define KROSS_FILE_LOG false
#endif
#ifndef KROSS_PROFILING
#define KROSS_PROFILING 0
#endif
#ifndef KROSS_RUNTIME_PROFILING
#define KROSS_RUNTIME_PROFILING 0
#endif
#ifndef KROSS_FILE_PROFILING
#define KROSS_FILE_PROFILING 0
#endif
#ifndef KROSS_GLAD
#define KROSS_GLAD 1
#endif
#ifndef KROSS_DLL
#define KROSS_DLL 0
#endif

#endif // !CORE_CONFIG

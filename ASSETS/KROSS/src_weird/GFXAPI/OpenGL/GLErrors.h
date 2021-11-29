#pragma once

#include "Kross/Core/Core.h"

#if KROSS_GLAD
#include <GLAD/include/glad/glad.h>	// Initialize with gladLoadGL()
#elif defined(KROSS_GL3W)
#include <GL3W/gl3w.h>	// Initialize with gl3wInit()
#elif defined(KROSS_GLEW)
#include <GLEW/glew.h>	// Initialize with glewInit()
#endif

#if KROSS_GL_DEBUG
    #define glCall(x) { KROSS_PROFILE_SCOPE(#x); x; glError error(#x, __FILE__, __LINE__);error.handle(); }
#elif KROSS_GLCALL_PROFILE
    #define glCall(x)                                       \
        {                                                   \
            KROSS_PROFILE_SCOPE(#x);                        \
            x;                                              \
        }                                                   
#else
    #define glCall(x)                                       \
        {                                                   \
            x;                                              \
        }                                                   
#endif

void GLAPIENTRY
glDebugMessage(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);

class glError
{
    static bool active;
    static unsigned int count;
	static std::vector<glError> cache;
	std::string func;
	std::string file;
	int line;
    std::vector<unsigned int> codes;
	unsigned int code, id;
public:

    glError(const std::string& func_, const std::string& file_, int line_);
    glError(const std::string& func_, const std::string& file_, int line_, unsigned int code_);

    static void glGetErrors(const char* function, const char* file, int line);
    static void Begin();
    static void flushCache();
    static bool handled(const glError& error);
    static void display(const glError& error);
    static void display(unsigned int code);

	bool hasError();
	bool handle();
    operator unsigned int();
}; 

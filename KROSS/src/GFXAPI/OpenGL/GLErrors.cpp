#include <Kross_pch.h>
#include "GLErrors.h"

std::vector<glError> glError::cache;
unsigned int glError::count = 0;
bool glError::active = false;


glError::glError(const std::string& func_, const std::string& file_, int line_)
	: func(func_),
	file(file_),
	line(line_),
	codes(),
	id(++count)
{
	if (active)
		code = glGetError();
	else
	{
		KROSS_ERROR("Trying to use glError without active instance of handler");
	}
}
glError::glError(const std::string& func_, const std::string& file_, int line_, unsigned int code_)
	: func(func_),
	file(file_),
	line(line_),
	codes(),
	code(code_),
	id(++count)
{}
void glError::Begin()
{
	if (cache.empty() && !active && count == 0)
		active = true;
	else
	{
		KROSS_ERROR("Trying to initilize OpenGL Error Handler with an already initialized instance.");
	}
}
void glError::flushCache()
{
	if (active)
	{
		cache.clear();
		count = 0;
		active = false;
	}
	else
	{
		KROSS_ERROR("Trying to flush OpenGL Error Handler without an valid instance.");
	}
}
bool glError::hasError()
{
	return code != GL_NO_ERROR;
}
bool glError::handle()
{
	if (active)
	{
		while (hasError() && !handled(*this))
		{
			codes.push_back(code);
			display(*this);
		}
	}
	else return false;
	return true;
}
void glError::display(const glError& error)
{
	KROSS_ERROR("FILE: {0}", error.file);
	KROSS_ERROR("LINE: {0}", error.line);
	KROSS_ERROR("FUNCTION: {0}", error.func);
}
bool glError::handled(const glError& error)
{
	auto i = std::lower_bound(cache.begin(), cache.end(), error,
		[](const glError& other, const glError& k) { return other.id < k.id; });
	if (i < cache.end() && !(i->id < error.id)) return true;
	else cache.emplace(i, error);
	return false;
}
glError::operator unsigned int()
{
	return code;
}
void glError::glGetErrors(const char* function, const char* file, int line)
{
	glError error(function, file, line);
	error.handle();
}

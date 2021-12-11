#pragma once

namespace Kross {
	void append(char* dest, size_t size, const char *str);
	const char *ReadFile(const char *filepath);
	const char *FileName(const char *path);
	int FlipBits(int &flags, int mask);
	int ClearBits(int &flags, int mask);
	int SetBits(int &flags, int mask);
}

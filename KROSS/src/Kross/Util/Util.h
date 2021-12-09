#pragma once

namespace Kross {
	const char *append(const char *str1, const char *str2);
	const char *ReadFile(const char *filepath);
	const char *FileName(const char *path);
	int FlipBits(int &flags, int mask);
	int ClearBits(int &flags, int mask);
	int SetBits(int &flags, int mask);
}

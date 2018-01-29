#ifndef NITROGEN_UTIL_H
#define NITROGEN_UTIL_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define error(...) fprintf(stderr, "\033[1;31m"); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\033[0m"); exit(1);

namespace Nitrogen {

	class Util {
	public:
		// Files
		static char* readFile(const char* path, int* length);

		// Numbers
		static bool isNumber(const char* str);
		static int toNum(const char* str, int base);
	};

}

#endif // NITROGEN_UTIL_H
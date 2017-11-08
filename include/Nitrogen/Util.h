#ifndef NITROGEN_UTIL_H
#define NITROGEN_UTIL_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace Nitrogen {

	class Util {
	public:
		static char* readFile(const char* path, int* length);
		static bool isNumber(const char* str);
		static int toNum(const char* str, int base);
	};

}

#endif // NITROGEN_UTIL_H
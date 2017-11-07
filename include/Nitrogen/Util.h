#ifndef NITROGEN_UTIL_H
#define NITROGEN_UTIL_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace Nitrogen {

	class Util {
	public:
		static char* readFile(const char* path, int* length);
	};

}

#endif // NITROGEN_UTIL_H
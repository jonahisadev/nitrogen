#include <Nitrogen/Util.h>

namespace Nitrogen {

	char* Util::readFile(const char* path, int* length) {
		FILE* file = fopen(path, "r");
		if (!file) {
			printf("ERR: Could not read file '%s'\n", path);
			exit(1);
		}
		
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		fseek(file, 0, SEEK_SET);
		*length = size;
		
		char* buffer = new char[size];
		fread(buffer, 1, size, file);
		fclose(file);
		
		buffer[size] = '\0';
		return buffer;
	}

}
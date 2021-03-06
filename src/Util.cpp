#include <Nitrogen/Util.h>

namespace Nitrogen {

	char* Util::readFile(const char* path, int* length) {
		FILE* file = fopen(path, "r");
		if (!file) {
			error("Could not read file '%s'\n", path);
			exit(1);
		}
		
		fseek(file, 0, SEEK_END);
		// int size = ftell(file);
		*length = ftell(file);
		fseek(file, 0, SEEK_SET);
		// *length = size;
		
		if (*length <= 0) {
			error("Invalid file read ('%s')\n", path);
		}

		char* buffer = new char[*length+1];
		fread(buffer, 1, *length, file);
		fclose(file);
		
		buffer[*length] = '\0';
		return buffer;
	}
	
	bool Util::isNumber(const char* str) {
		for (int i = 0; i < strlen(str); i++) {
			if (str[i] > '9' || str[i] < '0')
				return false;
		}
		return true;
	}
	
	int Util::toNum(const char* str, int base) {
		return (int)strtol(str, (char**)nullptr, base);
	}

	char* Util::createTempBuffer() {
		char* buf = new char[TEMP_BUF_SIZE];
		clearTempBuffer(buf);
		return buf;
	}

	void Util::clearTempBuffer(char* buf) {
		for (int i = 0; i < TEMP_BUF_SIZE; i++)
			buf[i] = '\0';
	}

}
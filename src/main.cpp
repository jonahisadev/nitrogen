#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <Nitrogen/Util.h>
#include <Nitrogen/Parser.h>

using namespace Nitrogen;

int main(int argc, char** argv) {
	if (argc < 3) {
		printf("Too few arguments!\n");
		exit(1);
	}
	
	if (!strcmp(argv[1], "c")) {
		int length;
		char* source = Util::readFile(argv[2], &length);
		Parser* p = new Parser(source, length);
		p->start();
		delete p;
	}
	
	return 0;
}
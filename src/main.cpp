#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include <chrono>

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
		
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		
		Parser* p = new Parser(strdup(source), length);
		p->start();
		
		Context* c = p->createContext();
		c->start();
		
		Compiler* comp = c->createCompiler();
		comp->start();
		
		delete comp;
		delete c;
		delete p;
		
		system("nvmc -c out.nvm");
		//system("rm -rf out.nvm");
		
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> time_span = t2 - t1;
		
		#ifdef DEBUG
			printf("Compile Time: %.0f ms\n", round(time_span.count()));
		#endif
	}
	
	return 0;
}
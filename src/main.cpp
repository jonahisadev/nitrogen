#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include <chrono>

#include <Nitrogen/Util.h>
#include <Nitrogen/Parser.h>

#include <Nitrogen/LinkedList.h>

using namespace Nitrogen;

#define CURRENT() current->data
#define REL(x) list->getRelative(x)->data

int main(int argc, char** argv) {
	if (argc < 3) {
		error("Too few arguments!\n");
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
		
		system("nvm -c out.nvm -g");
		//system("rm -rf out.nvm");
		
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> time_span = t2 - t1;

		#ifdef DEBUG
			printf("\033[1;32mCompile Time: %.0f ms\033[0m\n", round(time_span.count()));
		#endif
	}

	else if (!strcmp(argv[1], "n")) {
		// TODO: Native parsing
	}

	else if (!strcmp(argv[1], "ll")) {
		LinkedList<Token*>* list = new LinkedList<Token*>(nullptr);

		list->add(new Token(NUMBER, 40, 50));
		list->add(new Token(NUMBER, 2, 30));

		LinkData<Token*>* current = list->get(0);

		current->insertAfter(new LinkData<Token*>(nullptr, nullptr, new Token(OP, PLUS, 0)));

		while (current->hasChild()) {
			if (CURRENT()->getType() == OP && CURRENT()->getData() == PLUS &&
					REL(-1)->getType() == NUMBER && REL(1)->getType() == NUMBER) {
				int x = REL(-1)->getData();
				int y = REL(1)->getData();
				printf("%d + %d = %d\n", x, y, x + y);
			}

			current = list->child(current);
		}

		// LinkData<Token*>* ld = list->get(1);
		// LinkData<Token*>* ld2 = list->getRelative(1);
		// Token* t = ld2->data;
		// printf("Type: %d, Data: %d, Line: %d\n", t->getType(), t->getData(), t->getLine());
	}
	
	return 0;
}
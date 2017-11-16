#include <Nitrogen/Context.h>

namespace Nitrogen {

	Context::Context() {
		this->symbols = new List<char*>(1);
	}
	
	Context::~Context() {
		// Nothing
	}

	void Context::start() {
		Token* t;
		
		for (int i = 0; i < tokens->getSize(); i++) {
			t = tokens->get(i);
			
			if (t->getType() == NAME) {
				// Function Name
				if (tokens->get(i-1)->getType() == KEYWORD &&
						tokens->get(i-1)->getData() == FUNC &&
						tokens->get(i+1)->getType() == SPECIAL &&
						tokens->get(i+1)->getData() == LEFT_PAR) {
					symbols->add(names->get(t->getData()));
					t->setType(ID);
					t->setData(symbols->getSize()-1);
					verifySymbol(t);
				}
				
				// Function Type
				else if (tokens->get(i-1)->getType() == SPECIAL &&
						tokens->get(i-1)->getData() == LEFT_BRACK &&
						tokens->get(i+1)->getType() == SPECIAL &&
						tokens->get(i+1)->getData() == RIGHT_BRACK) {
					t->setType(TYPE);
				}
			}
		}
	}
	
	void Context::verifySymbol(Token* t) {
		int index = t->getData();
		char* sym = symbols->get(index);
		for (int i = 0; i < symbols->getSize(); i++) {
			if (i == index)
				continue;
			if (!strcmp(sym, symbols->get(i))) {
				printf("(%d) Duplicate symbol '%s'\n", t->getLine(), sym);
				exit(1);
			}
		}
	}
	
	Compiler* Context::createCompiler() {
		Compiler* c = new Compiler();
		c->setTokens(tokens);
		c->setNames(names);
		c->setSymbols(symbols);
		return c;
	}

}
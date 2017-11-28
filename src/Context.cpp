#include <Nitrogen/Context.h>

namespace Nitrogen {

	Context::Context() {
		this->symbols = new List<char*>(1);
		this->gvars = new List<Variable*>(1);
	}
	
	Context::~Context() {
 		delete this->symbols;
		delete this->gvars;
	}

	void Context::start() {
		Token* t;
		
		for (int i = 0; i < tokens->getSize(); i++) {
			t = tokens->get(i);
			
			if (t->getType() == NAME) {
				
				int index;

				// If it's not global, it's local
				if (tokens->get(i+1)->getType() == SPECIAL && tokens->get(i+1)->getData() == EQUALS &&
						tokens->get(i+2)->getType() == NUMBER) {
					t->setType(LVAR);
				}
					// Function Name
				else if (i != 0 && tokens->get(i - 1)->getType() == KEYWORD &&
						 tokens->get(i - 1)->getData() == FUNC &&
						 tokens->get(i + 1)->getType() == SPECIAL &&
						 tokens->get(i + 1)->getData() == LEFT_PAR)
				{
					addSymbol(t);
				}

				// Function Type
				else if (i != 0 && tokens->get(i - 1)->getType() == SPECIAL &&
						tokens->get(i - 1)->getData() == LEFT_BRACK &&
						tokens->get(i + 1)->getType() == SPECIAL &&
						tokens->get(i + 1)->getData() == RIGHT_BRACK)
				{
					t->setType(TYPE);
				}

				// Function Parameter
				else if (i != 0 &&tokens->get(i - 1)->getType() == SPECIAL && tokens->get(i - 1)->getData() == LEFT_PAR &&
						tokens->get(i + 1)->getType() == SPECIAL && tokens->get(i + 1)->getData() == COLON &&
						tokens->get(i + 2)->getType() == TYPE)
				{
					// printf("first param\n");
					t->setType(PVAR);
				}
				else if (i != 0 && tokens->get(i - 1)->getType() == SPECIAL && tokens->get(i - 1)->getData() == COMMA &&
						tokens->get(i + 1)->getType() == SPECIAL && tokens->get(i + 1)->getData() == COLON &&
						tokens->get(i + 2)->getType() == TYPE)
				{
					// printf("second param\n");
					t->setType(PVAR);
				}

				// Variable Declaration
				else if (tokens->get(i + 1)->getType() == SPECIAL &&
					tokens->get(i + 1)->getData() == COLON &&
					tokens->get(i + 2)->getType() == TYPE)
				{
					addGlobalVariable(t, tokens->get(i + 2));
				}

				// Global Variable Reference
				if ((index = isGlobalVariable(names->get(t->getData()))) != -1)
				{
					t->setType(GVAR);
					t->setData(index);
					continue;
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
			// printf("sym compare '%s' and '%s'\n", sym, symbols->get(i));
			if (!strcmp(sym, symbols->get(i))) {
				printf("(%d) Duplicate symbol '%s'\n", t->getLine(), sym);
				exit(1);
			}
		}
	}
	
	void Context::addSymbol(Token* t) {
		symbols->add(names->get(t->getData()));
		t->setType(ID);
		t->setData(symbols->getSize()-1);
		verifySymbol(t);
	}
	
	void Context::addGlobalVariable(Token* t, Token* type) {
		symbols->add(names->get(t->getData()));
		t->setData(symbols->getSize() - 1);
		verifySymbol(t);
		
		const char* name = symbols->get(symbols->getSize() - 1);
		Type* dtype = types->get(type->getData());
		
		gvars->add(new Variable(name, dtype));
		t->setType(GVAR);
		t->setData(gvars->getSize() - 1);
	}
	
	int Context::isGlobalVariable(char* name) {
		for (int i = 0; i < gvars->getSize(); i++) {
			if (!strcmp(name, gvars->get(i)->name)) {
				return i;
			}
		}
		return -1;
	}
	
	Compiler* Context::createCompiler() {
		Compiler* c = new Compiler();
		c->setTokens(tokens);
		c->setNames(names);
		c->setSymbols(symbols);
		c->setTypes(types);
		c->setGlobalVariables(gvars);
		return c;
	}

}
#include <Nitrogen/Context.h>

namespace Nitrogen {

	Context::Context() {
		this->ids = new List<char*>(1);
	}
	
	Context::~Context() {
 		delete this->ids;
	}

	void Context::start() {
		Token* t;
		
		for (int i = 0; i < tokens->getSize(); i++) {
			t = tokens->get(i);

			if (t->getType() == NAME) {

				// Variable declaration
				if (tokens->get(i+1)->getType() == SPECIAL && tokens->get(i+1)->getData() == COLON) {
					t->setType(VAR);
				}

				// Variable setting
				else if (tokens->get(i+1)->getType() == SPECIAL && tokens->get(i+1)->getData() == EQUALS) {
					t->setType(VAR);
				}

				// Function Declaration
				else if (tokens->get(i-1)->getType() == KEYWORD && tokens->get(i-1)->getData() == FUNC) {
					ids->add(names->get(t->getData()));
					t->setType(ID);
					t->setData(ids->getSize() - 1);
				}

				// Function Call
				else if (tokens->get(i+1)->getType() == SPECIAL && tokens->get(i+1)->getData() == LEFT_PAR) {
					t->setType(ID);
				}

				// Return Variable
				else if (tokens->get(i-1)->getType() == KEYWORD && tokens->get(i-1)->getData() == RETURN) {
					t->setType(VAR);
				}

				else {
					t->setType(VAR);
				}

			}
		}
	}
	
	
	Compiler* Context::createCompiler() {
		Compiler* c = new Compiler();
		c->setTokens(tokens);
		c->setNames(names);
		c->setIDs(ids);
		c->setTypes(types);
		return c;
	}

}
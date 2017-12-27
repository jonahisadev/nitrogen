#include <Nitrogen/Context.h>

#define REL(x) tokens->getRelative(x)->data

namespace Nitrogen {

	Context::Context() {
		this->ids = new List<char*>(1);
	}
	
	Context::~Context() {
 		delete this->ids;
	}

	void Context::start() {
		Token* t;
		
		LinkData<Token*>* current = tokens->get(0);
		while (current != nullptr) {
			t = current->data;

			if (t->getType() == NAME) {

				// Variable declaration
				if (REL(1)->getType() == SPECIAL && REL(1)->getData() == COLON) {
					t->setType(VAR);
				}

				// Variable setting
				else if (REL(1)->getType() == OP && REL(1)->getData() == EQUALS) {
					t->setType(VAR);
				}

				// Function Declaration
				else if (REL(-1)->getType() == KEYWORD && REL(-1)->getData() == FUNC) {
					ids->add(names->get(t->getData()));
					t->setType(ID);
					t->setData(ids->getSize() - 1);
				}

				// Function Call
				else if (REL(1)->getType() == SPECIAL && REL(1)->getData() == LEFT_PAR) {
					t->setType(ID);
				}

				// Return Variable
				else if (REL(-1)->getType() == KEYWORD && REL(-1)->getData() == RETURN) {
					t->setType(VAR);
				}

				// Hack
				else {
					t->setType(VAR);
				}

			}

			current = tokens->child(current);
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
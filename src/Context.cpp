#include <Nitrogen/Context.h>

#define REL(x) tokens->getRelative(x)->data

namespace Nitrogen {

	Context::Context() {
		this->ids = new List<char*>(1);
		this->exprs = new List<Expression*>(1);
	}
	
	Context::~Context() {
 		delete this->ids;
	}

	void Context::start() {
		Token* t;
		
		LinkData<Token*>* current = tokens->get(0);
		while (current != nullptr) {
			t = current->data;

			if (t->getType() == OP && t->getData() == EQUALS) {
				createExpression(current->child);
			}

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
	
	void Context::createExpression(LinkData<Token*>* start) {
		LinkData<Token*>* current = start;
		int line = start->data->getLine();
		Expression* e = new Expression();

		int count;
		Token* t;
		while (current->data->getLine() == line) {
			t = current->data;
			if (t->getType() == NUMBER) {
				// printf("adding number %d\n", t->getData());
				e->addNumber(t->getData());
			}
			else if (t->getType() == OP) {
				// printf("adding operator %d\n", t->getData());
				e->addOp(t->getData());
			}

			count++;
			current = tokens->child(current);
		}
		
		// Create expression token
		start->insertBefore(new LinkData<Token*>(nullptr, nullptr, new Token(EXPR, exprs->getSize(), line)));
		exprs->add(e);
		LinkData<Token*>* exprToken = start->child;

		// Link the two important tokens
		exprToken->child = current;
		current->parent = exprToken;

		// TODO: Free memory from other tokens that aren't in use anymore
	}
	
	Compiler* Context::createCompiler() {
		Compiler* c = new Compiler();
		c->setTokens(tokens);
		c->setNames(names);
		c->setIDs(ids);
		c->setTypes(types);
		c->setExpressions(exprs);
		return c;
	}

}
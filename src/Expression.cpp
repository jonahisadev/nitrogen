#include <Nitrogen/Expression.h>

namespace Nitrogen {

	// 	Type = -1: Root
	// 	Type =  0: Register
	//		Data = 1: EAX
	//		Data = 2: EBX
	//		Data = 3: ECX
	//		Data = 4: EDX

	Expression::Expression() {
		this->root = new ExprNode(-1, 0);
		this->current = this->root;
	}

	Expression::~Expression() {
		// TODO: Traverse tree and delete nodes
	}

	void Expression::addOp(int data) {
		if (this->current->type == -1) {
			this->current->type = OP;
			this->current->data = data;
		} else {
			ExprNode* e = new ExprNode(OP, data);
			this->current->parent = e;
			e->addChild(this->current);
			this->current = e;
		}
	}

	void Expression::addNumber(int data) {
		ExprNode* e = new ExprNode(NUMBER, data);
		this->current->addChild(e);
	}

	void Expression::addVariable(int data) {
		ExprNode* e = new ExprNode(VAR, data);
		this->current->addChild(e);
	}

	List<char*>* Expression::evaluate(FILE* out, Compiler* compiler) {
		ExprNode* current = this->root;
		List<char*>* lines = new List<char*>(1);
		char* temp = Util::createTempBuffer();

		/*
		enum regs {
			EAX,
			EBX,
			ECX,
			EDX
		};
		int rindex;
		*/

		// printf("began expression parsing\n");

		while (true) {
			if (current == nullptr)
				break;

			// Just one thing
			if (current->type == -1) {
				if (current->childCount() == 1) {

					if (current->left->type == NUMBER) {
						sprintf(temp, "\tmov eax %d\n", current->left->data);
						lines->add(strdup(temp));
						Util::clearTempBuffer(temp);
						break;
					}

				}
			}

			if (current->type == OP) {
				if (current->childCount() != 2) {
					error("FATAL: Expression tree has a dead leaf\n");
				}

				ExprNode* left = current->left;
				ExprNode* right = current->right;

				// printf("left: (%d, %d), right: (%d, %d)\n", left->type, left->data, right->type, right->data);

				// NUMBERS
				if (left->type == NUMBER && right->type == NUMBER) {
					sprintf(temp, "\tmov eax %d\n", left->data);
					lines->add(strdup(temp));
					Util::clearTempBuffer(temp);

					sprintf(temp, "\t%s eax %d\n", getOpName(current->data), right->data);
					lines->add(strdup(temp));
					Util::clearTempBuffer(temp);

					current->type = 0;
					current->data = 1;
					current = current->parent;
				}

				// NUMBER AND GVAR
				else if ((left->type == NUMBER && right->type == VAR) ||
						(left->type == VAR && right->type == NUMBER)) {
					ExprNode* num = getNumber(left, right);
					ExprNode* var = getVariable(left, right);

					if (num == nullptr || var == nullptr) {
						error("Number & GVAR: one of these is not what we thought!\n");
					}

					// Load register with variable contents
					Variable* gvar = compiler->getGlobalVariable(var->data);
					sprintf(temp, "\tld%s eax $g_%s\n", compiler->getStoreSize(gvar), gvar->name);
					lines->add(strdup(temp));
					Util::clearTempBuffer(temp);

					// Do math
					sprintf(temp, "\t%s eax %d\n", getOpName(current->data), num->data);
					lines->add(strdup(temp));
					Util::clearTempBuffer(temp);

					current->type = 0;
					current->data = 1;
					current = current->parent;
				}

				// NUMBER AND REGISTER
				else if ((left->type == NUMBER && right->type == 0) ||
						(left->type == 0 && right->type == NUMBER)) {
					ExprNode* num = getNumber(left, right);
					
					if (num == nullptr) {
						error("Number & Register: one of these is not what we thought!\n");
					}

					sprintf(temp, "\t%s eax %d\n", getOpName(current->data), num->data);
					lines->add(strdup(temp));
					Util::clearTempBuffer(temp);

					current->type = 0;
					current->data = 1;
					current = current->parent;
				}

				// TRAVERSE
				else if (left->type == OP || right->type == OP) {
					if (left->type == OP)
						current = left;
					else
						current = right;
				}
			}
		}

		delete[] temp;
		return lines;
	}

	const char* Expression::getOpName(int data) {
		switch (data) {
			case PLUS:
				return "add";
			case MINUS:
				return "sub";
			default:
				return nullptr;
		}
	}

	ExprNode* Expression::getNumber(ExprNode* a, ExprNode* b) {
		if (a->type == NUMBER)
			return a;
		else if (b->type == NUMBER)
			return b;
		else
			return nullptr;
	}

	ExprNode* Expression::getVariable(ExprNode* a, ExprNode* b) {
		if (a->type == VAR)
			return a;
		else if (b->type == VAR)
			return b;
		else
			return nullptr;
	}

}
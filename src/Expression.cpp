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

	void Expression::evaluate(FILE* out) {
		ExprNode* current = this->root;

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
						fprintf(out, "\tmov eax %d\n", current->left->data);
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
					fprintf(out, "\tmov eax %d\n", left->data);
					fprintf(out, "\t%s eax %d\n", getOpName(current->data), right->data);

					current->type = 0;
					current->data = 1;
					current = current->parent;
				}

				// NUMBER AND REGISTER
				if ((left->type == NUMBER && right->type == 0) ||
						(left->type == 0 && right->type == NUMBER)) {
					ExprNode* num = getNumber(left, right);
					fprintf(out, "\t%s eax %d\n", getOpName(current->data), num->data);

					current->type = 0;
					current->data = 1;
					current = current->parent;
				}

				// TRAVERSE
				if (left->type == OP || right->type == OP) {
					if (left->type == OP)
						current = left;
					else
						current = right;
				}
			}
		}
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

}
#include <Nitrogen/Expression.h>

namespace Nitrogen {

	Expression::Expression() {
		this->root = new ExprNode(0, 0);
		this->current = this->root;
	}

	Expression::~Expression() {
		// TODO: Traverse tree and delete nodes
	}

	void Expression::addOp(int data) {
		if (this->current->type == 0) {
			this->current->type = OP;
			this->current->data = data;
		}
	}

	void Expression::addNumber(int data) {
		ExprNode* e = new ExprNode(NUMBER, data);
		this->current->addChild(e);
	}

	void Expression::evaluate(FILE* out) {
		ExprNode* current = this->root;

		while (true) {
			if (current->type == OP) {
				ExprNode* left = current->left;
				ExprNode* right = current->right;

				// printf("left: (%d, %d), right: (%d, %d)\n", left->type, left->data, right->type, right->data);

				if (left->type == NUMBER && right->type == NUMBER) {
					fprintf(out, "\tmov eax %d\n", left->data);
					fprintf(out, "\t%s eax %d\n", getOpName(current->data), right->data);
				}
			}

			if (current->parent == nullptr) {
				break;
			}
		}
	}

	const char* Expression::getOpName(int data) {
		switch (data) {
			case PLUS:
				return "add";
			default:
				return nullptr;
		}
	}

}
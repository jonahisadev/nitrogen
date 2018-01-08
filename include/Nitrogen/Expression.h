#ifndef NITROGEN_EXPRESSION_H
#define NITROGEN_EXPRESSION_H

#include <Nitrogen/Token.h>

#include <cstdio>

namespace Nitrogen {

	struct ExprNode {
		ExprNode* parent;
		ExprNode* left;
		ExprNode* right;

		int type;
		int data;

		ExprNode(int type, int data) {
			this->type = type;
			this->data = data;
		}

		void addChild(ExprNode* en) {
			if (!left) {
				this->left = en;
			} else {
				this->right = en;
			}
		}
	};

	class Expression {
	private:
		ExprNode* root;
		ExprNode* current;

	public:
		Expression();
		~Expression();

		void addOp(int data);
		void addNumber(int data);

		void evaluate(FILE* out);
		const char* getOpName(int data);
		ExprNode* getRoot() const { return root; }
	};

}

#endif // NITROGEN_EXPRESSION_H
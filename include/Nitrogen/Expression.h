#ifndef NITROGEN_EXPRESSION_H
#define NITROGEN_EXPRESSION_H

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
	};

	class Expression {
	private:
		char* str;
		ExprNode* root;

	public:
		Expression(char* str);
		~Expression();

		void evaluate();
		ExprNode* getRoot() const { return root; }
	};

}

#endif // NITROGEN_EXPRESSION_H
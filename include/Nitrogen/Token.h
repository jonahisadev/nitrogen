#ifndef NITROGEN_TOKEN_H
#define NITROGEN_TOKEN_H

#include <Nitrogen/List.h>
#include <Nitrogen/LinkedList.h>

namespace Nitrogen {

	enum TokenType {
		KEYWORD,
		NUMBER,
		SPECIAL,
		OP,
		EXPR,
		
		NAME,
		ID,
		TYPE,
		
		VAR,		// Generic
		GVAR,		// Global
		PVAR,		// Parameter
		LVAR,		// Local
	};
	
	enum TokenKeyword {
		FUNC,
		RETURN,
		ENDF,
		NATIVE,
		STRUCT,
		END,
	};
	
	enum TokenSpecial {
		LEFT_PAR,
		RIGHT_PAR,
		LEFT_BRACK,
		RIGHT_BRACK,
		COLON,
		COMMA,
		EXCLAIM,
		DOT,
		HASH,
		STAR,
	};

	enum TokenOperators {
		EQUALS,
		PLUS,
		MINUS,
	};

	class Token;
	template class List<Token*>;
	template class LinkedList<Token*>;

	class Token {
	private:
		int type;
		int data;
		int line;
		
	public:
		Token(int type, int data, int line);
		~Token();
		
		int getType() const { return type; }
		int getData() const { return data; }
		int getLine() const { return line; }
		
		void setType(int type) { this->type = type; }
		void setData(int data) { this->data = data; }
	};

}

#endif // NITROGEN_TOKEN_H
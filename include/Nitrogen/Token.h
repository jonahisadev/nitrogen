#ifndef NITROGEN_TOKEN_H
#define NITROGEN_TOKEN_H

#include <Nitrogen/List.h>

namespace Nitrogen {

	enum TokenType {
		KEYWORD,
		NUMBER,
		SPECIAL,
		
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
	};
	
	enum TokenSpecial {
		LEFT_PAR,
		RIGHT_PAR,
		LEFT_BRACK,
		RIGHT_BRACK,
		COLON,
		EQUALS,
		COMMA,
		EXCLAIM,
		DOT,
	};

	class Token;
	template class List<Token*>;

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
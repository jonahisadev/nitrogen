#ifndef NITROGEN_PARSER_H
#define NITROGEN_PARSER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <Nitrogen/List.h>
#include <Nitrogen/Token.h>
#include <Nitrogen/Util.h>
#include <Nitrogen/Context.h>
#include <Nitrogen/LinkedList.h>

namespace Nitrogen {

	class Parser {
	private:
		char* source;
		int length;
		
		// List<Token*>* tokens;
		LinkedList<Token*>* tokens;

		List<char*>* names;
		List<Type*>* types;

		bool comment = false;
		
	public:
		Parser(char* source, int length);
		~Parser();
		
		void start();
		bool isImportant(char c);
		int isKeyword(char* str);
		int isType(char* str);
		
		Context* createContext();
		
		// int getListSize() const { return tokens->getSize(); }
	};

}

#endif // NITROGEN_PARSER_H
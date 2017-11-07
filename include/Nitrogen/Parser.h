#ifndef NITROGEN_PARSER_H
#define NITROGEN_PARSER_H

#include <cstdio>
#include <Nitrogen/List.h>
#include <Nitrogen/Token.h>

namespace Nitrogen {

	class Parser {
	private:
		char* source;
		int length;
		
	public:
		Parser(char* source, int length);
		~Parser();
		
		void start();
	};

}

#endif // NITROGEN_PARSER_H
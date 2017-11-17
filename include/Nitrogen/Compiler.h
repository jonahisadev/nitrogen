#ifndef NITROGEN_COMPILER_H
#define NITROGEN_COMPILER_H

#include <cstdio>
#include <cstdlib>

#include <Nitrogen/Util.h>
#include <Nitrogen/Token.h>
#include <Nitrogen/List.h>
#include <Nitrogen/Template.h>

namespace Nitrogen {

	class Compiler {
	private:
		FILE* out;
		
		List<Token*>* tokens;
		List<char*>* names;
		List<char*>* symbols;
		
		List<char*>* varBuffer;
		
	public:
		Compiler();
		~Compiler();
		
		void start();
		
		void setTokens(List<Token*>* tokens) { this->tokens = tokens; };
		void setNames(List<char*>* names) { this->names = names; };
		void setSymbols(List<char*>* symbols) { this->symbols = symbols; };
	};

}

#endif // NITROGEN_COMPILER_H
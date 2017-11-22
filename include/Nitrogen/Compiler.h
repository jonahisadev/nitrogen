#ifndef NITROGEN_COMPILER_H
#define NITROGEN_COMPILER_H

#include <cstdio>
#include <cstdlib>

#include <Nitrogen/Util.h>
#include <Nitrogen/Token.h>
#include <Nitrogen/List.h>
#include <Nitrogen/Template.h>
#include <Nitrogen/Type.h>

namespace Nitrogen {

	class Compiler {
	private:
		FILE* out;
		
		List<Token*>* tokens;
		List<char*>* names;
		List<char*>* symbols;
		List<Variable*>* gvars;
		
		List<char*>* varBuffer;
		
	public:
		Compiler();
		~Compiler();
		
		void start();
		
		void setTokens(List<Token*>* tokens) { this->tokens = tokens; };
		void setNames(List<char*>* names) { this->names = names; };
		void setSymbols(List<char*>* symbols) { this->symbols = symbols; };
		void setGlobalVariables(List<Variable*>* gvars) { this->gvars = gvars; }
		
		const char* getStoreSize(Token* var);
		const char* getLoadSize(Token* var);
	};

}

#endif // NITROGEN_COMPILER_H
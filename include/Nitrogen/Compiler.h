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
		List<char*>* funcs;
		List<Type*>* types;
		List<Variable*>* gvars;
		
		List<char*>* varBuffer;

		Function* currentFunction = nullptr;
		
	public:
		Compiler();
		~Compiler();
		
		void start();
		void getNatives();
		Function* createFunction(Token* name, int* index);

		void setTokens(List<Token*>* tokens) { this->tokens = tokens; };
		void setNames(List<char*>* names) { this->names = names; };
		void setFuncs(List<char*>* funcs) { this->funcs = funcs; };
		void setTypes(List<Type*>* types) { this->types = types; }
		
		const char* getStoreSize(Variable* var);
		const char* getInstSize(Variable* v);

		int isGlobal(char* name);
	};

}

#endif // NITROGEN_COMPILER_H
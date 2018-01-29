#ifndef NITROGEN_COMPILER_H
#define NITROGEN_COMPILER_H

#include <cstdio>
#include <cstdlib>

#include <Nitrogen/Util.h>
#include <Nitrogen/Token.h>
#include <Nitrogen/List.h>
#include <Nitrogen/Template.h>
#include <Nitrogen/Type.h>
#include <Nitrogen/Expression.h>
#include <Nitrogen/Struct.h>

namespace Nitrogen {

	class Compiler {
	private:
		FILE* out;
		
		LinkedList<Token*>* tokens;
		List<char*>* names;
		List<char*>* ids;
		List<Type*>* types;
		List<Variable*>* gvars;
		List<Function*>* funcs;
		List<Expression*>* exprs;
		List<Struct*>* structs;
		
		List<char*>* varBuffer;

		Function* currentFunction = nullptr;
		
	public:
		Compiler();
		~Compiler();
		
		void start();
		void getNatives();

		Function* createFunction(LinkData<Token*>* func);
		void parseFunctionCall(LinkData<Token*>* func);
		Struct* createStruct(LinkData<Token*>* s, int* offset);

		void setTokens(LinkedList<Token*>* tokens) { this->tokens = tokens; };
		void setNames(List<char*>* names) { this->names = names; };
		void setIDs(List<char*>* ids) { this->ids = ids; };
		void setTypes(List<Type*>* types) { this->types = types; }
		void setExpressions(List<Expression*>* exprs) { this->exprs = exprs; }
		
		const char* getStoreSize(Variable* var);
		const char* getInstSize(Variable* v);

		int isFunction(char* name);
		int isGlobal(char* name);
	};

}

#endif // NITROGEN_COMPILER_H
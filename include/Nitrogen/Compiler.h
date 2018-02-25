#ifndef NITROGEN_COMPILER_H
#define NITROGEN_COMPILER_H

#include <cstdio>
#include <cstdlib>

#include <Nitrogen/Util.h>
#include <Nitrogen/Token.h>
#include <Nitrogen/List.h>
#include <Nitrogen/Template.h>
#include <Nitrogen/Type.h>
#include <Nitrogen/Struct.h>
#include <Nitrogen/Expression.h>

namespace Nitrogen {

	class Expression;

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
		List<char*>* initBuffer;

		Function* currentFunction = nullptr;
		
	public:
		Compiler();
		~Compiler();
		
		void start();
		void getNatives();

		Function* createFunction(LinkData<Token*>* func);
		void parseFunctionCall(LinkData<Token*>* func);
		Struct* createStruct(LinkData<Token*>* s, int* offset);
		void createStructConstructor(Struct* s, Function* f);

		void setTokens(LinkedList<Token*>* tokens) { this->tokens = tokens; };
		void setNames(List<char*>* names) { this->names = names; };
		void setIDs(List<char*>* ids) { this->ids = ids; };
		void setTypes(List<Type*>* types) { this->types = types; }
		void setExpressions(List<Expression*>* exprs) { this->exprs = exprs; }
		
		const char* getStoreSize(Variable* var);
		const char* getInstSize(Variable* v);

		// For expression parser
		Variable* getGlobalVariable(int index) { return this->gvars->get(index); }
		Function* getCurrentFunction() { return this->currentFunction; }
		char* getName(int index) { return this->names->get(index); }

		int isFunction(char* name);
		int isGlobal(char* name);
	};

}

#endif // NITROGEN_COMPILER_H
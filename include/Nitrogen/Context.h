#ifndef NITROGEN_CONTEXT_H
#define NITROGEN_CONTEXT_H

#include <cstdlib>
#include <cstring>

#include <Nitrogen/Util.h>
#include <Nitrogen/List.h>
#include <Nitrogen/Token.h>
#include <Nitrogen/Compiler.h>
#include <Nitrogen/Type.h>

namespace Nitrogen {

	class Context {
	private:
		List<Token*>* tokens;
		List<char*>* names;
		List<char*>* funcs;
		List<Type*>* types;
		
	public:
		Context();
		~Context();
		
		void start();
		
		Compiler* createCompiler();
		
		void setTokens(List<Token*>* tokens) { this->tokens = tokens; };
		void setNames(List<char*>* names) { this->names = names; };
		void setTypes(List<Type*>* types) { this->types = types; }
	};

}

#endif // NITROGEN_CONTEXT_H
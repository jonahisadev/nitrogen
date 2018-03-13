#ifndef NITROGEN_STRUCT_H
#define NITROGEN_STRUCT_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <Nitrogen/Util.h>
#include <Nitrogen/List.h>
#include <Nitrogen/Type.h>

namespace Nitrogen {

	class Struct {
	public:
		char* name;
		List<Variable*>* vars;
		List<Function*>* funcs;
		int size;

		Struct(char* name);
		~Struct();

		void addVariable(Variable* v);
		Variable* getVariable(char* str);
		Function* getFunction(const char* str);
	};
	template class List<Struct*>;

}

#endif // NITROGEN_STRUCT_H
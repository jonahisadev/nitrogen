#ifndef NITROGEN_TYPE_H
#define NITROGEN_TYPE_H

#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <Nitrogen/List.h>

namespace Nitrogen {

	struct Type {
		const char* name;
		int size;
		
		Type(const char* name, int size) {
			this->name = strdup(name);
			this->size = size;
		}
		
		~Type() {
			delete this->name;
		}
	};
	
	struct Variable {
		const char* name;
		Type* type;
		// int data;
		
		Variable(const char* name, Type* type) {
			this->name = strdup(name);
			this->type = type;
			// this->data = data;
		}
		
		~Variable() {
			delete this->name;
		}
	};
	template class List<Variable*>;
	
	enum FunctionType {
		F_LOCAL,
		F_NATIVE,
	};

	struct Function {
		const char* name;
		Type* ret;
		List<Variable*>* params;
		List<Variable*>* locals;
		int type = F_LOCAL;
		
		Function(const char* name) {
			this->name = strdup(name);
			this->params = new List<Variable*>(1);
			this->locals = new List<Variable*>(1);
		}
		
		~Function() {
			delete this->name;
		}

		int isParam(const char* str) {
			for (int i = 0; i < params->getSize(); i++) {
				if (!strcmp(params->get(i)->name, str)) {
					return i;
				}
			}
			return -1;
		}

		int getParamOffset(int index) {
			int size = 0;
			for (int i = 0; i < index; i++) {
				size += params->get(i)->type->size;
			}
			return size;
		}
		
		void setReturnType(Type* ret) { this->ret = ret; }
		void addParam(Variable* param) { this->params->add(param); }
		void addLocal(Variable* local) { this->locals->add(local); }
		void setType(int type) { this->type = type; };
	};

}

#endif // NITROGEN_TYPE_H
#ifndef NITROGEN_TYPE_H
#define NITROGEN_TYPE_H

#include <cstring>
#include <cstdlib>
#include <cstdio>

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

}

#endif // NITROGEN_TYPE_H
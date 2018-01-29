#include <Nitrogen/Struct.h>

namespace Nitrogen {

	Struct::Struct(char* name) {
		this->name = name;
		this->vars = new List<Variable*>(1);
	}

	Struct::~Struct() {
		for (int i = 0; i < vars->getSize(); i++) {
			delete vars->get(i);
		}
	}

	void Struct::addVariable(Variable* v) {
		vars->add(v);
	}

	Variable* Struct::getVariable(char* str) {
		for (int i = 0; i < vars->getSize(); i++) {
			if (!strcmp(vars->get(i)->name, str)) {
				return vars->get(i);
			}
		}
		error("Struct '%s' has no member '%s'\n", this->name, str);
	}

}
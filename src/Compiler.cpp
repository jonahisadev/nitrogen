#include <Nitrogen/Compiler.h>

#define RTOKEN(x) tokens->get(i+x)

namespace Nitrogen {

	Compiler::Compiler() {
		out = fopen("out.nvm", "w");
		this->varBuffer = new List<char*>(1);
		this->gvars = new List<Variable*>(1);
	}
	
	Compiler::~Compiler() {
		delete this->varBuffer;
		delete this->gvars;
	}
	
	void Compiler::start() {
		fprintf(out, "%s", VM_START);
		
		Token* t;
		
		for (int i = 0; i < tokens->getSize(); i++) {
			t = tokens->get(i);

			if (currentFunction == nullptr) {

				// Global Variable Declaration
				if (t->getType() == VAR && 
						RTOKEN(1)->getType() == SPECIAL && RTOKEN(1)->getData() == COLON &&
						RTOKEN(2)->getType() == TYPE &&
						RTOKEN(3)->getType() == SPECIAL && RTOKEN(3)->getData() == EXCLAIM) {
					Variable* v = new Variable(names->get(t->getData()), types->get(RTOKEN(2)->getData()));
					gvars->add(v);
					//t->setType(GVAR);

					char* buf = new char[256];
					sprintf(buf, VM_VAR_DEC_AND_INIT, v->name, getStoreSize(v), 0);
					this->varBuffer->add(strdup(buf));
					delete[] buf;
				}

				// Global Variable Declaration and Initialization
				else if (t->getType() == VAR && 
						RTOKEN(1)->getType() == SPECIAL && RTOKEN(1)->getData() == COLON &&
						RTOKEN(2)->getType() == TYPE &&
						RTOKEN(3)->getType() == SPECIAL && RTOKEN(3)->getData() == EQUALS &&
						RTOKEN(4)->getType() == NUMBER) {
					Variable* v = new Variable(names->get(t->getData()), types->get(RTOKEN(2)->getData()));
					gvars->add(v);
					//t->setType(GVAR);

					char* buf = new char[256];
					sprintf(buf, VM_VAR_DEC_AND_INIT, v->name, getStoreSize(v), RTOKEN(4)->getData());
					this->varBuffer->add(strdup(buf));
					delete[] buf;
				}

				// Function
				else if (t->getType() == KEYWORD && t->getData() == FUNC &&
						RTOKEN(1)->getType() == ID) {
					Function* f = createFunction(RTOKEN(1), &i);
					currentFunction = f;
					fprintf(out, VM_LABEL, f->name);
				}

			} else {
				int temp;

				// End Function
				if (t->getType() == KEYWORD && t->getData() == ENDF) {
					fprintf(out, "\t%s\n\n", "ret");
					currentFunction = nullptr;
				}

				// Return Variable
				else if (t->getType() == KEYWORD && t->getData() == RETURN &&
						RTOKEN(1)->getType() == VAR) {
					temp = isGlobal(names->get(RTOKEN(1)->getData()));
					if (temp != -1) {
						Variable* v = gvars->get(temp);
						fprintf(out, VM_RETURN_G_VAR, getStoreSize(v), v->name);
					} else if ((temp = currentFunction->isParam(names->get(RTOKEN(1)->getData()))) != -1) {
						Variable* v = currentFunction->params->get(temp);
						fprintf(out, VM_RETURN_P_VAR, getInstSize(v), currentFunction->getParamOffset(temp) + 8);
					}
				}

				// Return Constant
				else if (t->getType() == KEYWORD && t->getData() == RETURN &&
						RTOKEN(1)->getType() == NUMBER) {
					fprintf(out, VM_RETURN_CONST, RTOKEN(1)->getData());
				}

				// Set Global Variable
				else if (t->getType() == VAR && (temp = isGlobal(names->get(t->getData()))) != -1 &&
						RTOKEN(1)->getType() == SPECIAL && RTOKEN(1)->getData() == EQUALS &&
						RTOKEN(2)->getType() == NUMBER) {
					Variable* v = gvars->get(temp);
					fprintf(out, VM_VAR_SET, getStoreSize(v), v->name, RTOKEN(2)->getData());
				}

			}

		}

		fprintf(out, "%s\n", "#section DATA");
		for (int i = 0; i < varBuffer->getSize(); i++) {
			fprintf(out, "%s", varBuffer->get(i));
		}
		fprintf(out, "%s", "\n");
		fclose(out);
	}

	Function* Compiler::createFunction(Token* name, int* index) {
		Function* f = new Function(funcs->get(name->getData()));
		*index += 1;

		Token* t;
		int i;
		for (i = *index; i < tokens->getSize(); i++) {
			t = tokens->get(i);

			if (t->getType() == VAR &&
					RTOKEN(1)->getType() == SPECIAL && RTOKEN(1)->getData() == COLON &&
					RTOKEN(2)->getType() == TYPE) {
				char* name = names->get(t->getData());
				Type* type = types->get(RTOKEN(2)->getData());
				// printf("VAR: %s - %s\n", name, type->name);
				f->addParam(new Variable(name, type));
			}

			else if (RTOKEN(-1)->getType() == SPECIAL && RTOKEN(-1)->getData() == LEFT_BRACK &&
					t->getType() == TYPE &&
					RTOKEN(1)->getType() == SPECIAL && RTOKEN(1)->getData() == RIGHT_BRACK) {
				Type* type = types->get(t->getData());
				// printf("RET: %s\n", type->name);
				f->setReturnType(type);
				i += 2;
				break;
			}
		}

		*index = i;
		return f;
	}

	int Compiler::isGlobal(char* name) {
		for (int i = 0; i < gvars->getSize(); i++) {
			if (!strcmp(name, gvars->get(i)->name)) {
				return i;
			}
		}
		return -1;
	}

	const char* Compiler::getStoreSize(Variable* v) {
		switch (v->type->size) {
			case 4:
				return "d";
			case 2:
				return "w";
			case 1:
				return "b";
			default:
				return "d";
		}
	}

	const char* Compiler::getInstSize(Variable* v) {
		switch (v->type->size) {
			case 4:
				return "i";
			case 2:
				return "w";
			case 1:
				return "b";
			default:
				return "i";
		}
	}

}
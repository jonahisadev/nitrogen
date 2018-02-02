#include <Nitrogen/Compiler.h>

// #define RTOKEN(x) tokens->get(i+x)
#define RTOKEN(x) tokens->getRelative(x)->data

namespace Nitrogen {

	Compiler::Compiler() {
		out = fopen("out.nvm", "w");
		this->varBuffer = new List<char*>(1);
		this->initBuffer = new List<char*>(1);
		this->gvars = new List<Variable*>(1);
		this->funcs = new List<Function*>(1);
		this->structs = new List<Struct*>(1);
	}
	
	Compiler::~Compiler() {
		delete this->varBuffer;
		delete this->gvars;
		delete this->structs;
	}
	
	void Compiler::start() {
		fprintf(out, "%s", VM_START);
		
		Token* t;
		
		LinkData<Token*>* current = tokens->get(0);
		while (current != nullptr) {
			t = current->data;
			int temp;

			// printf("Token: {type=%d, data=%d, line=%d}\n", t->getType(), t->getData(), t->getLine());

			if (currentFunction == nullptr) {

				// printf("Out of function\n");

				// Struct
				if (t->getType() == KEYWORD && t->getData() == STRUCT &&
						RTOKEN(1)->getType() == ID &&
						RTOKEN(2)->getType() == SPECIAL && RTOKEN(2)->getData() == COLON) {
					current = tokens->child(current);
					int offset = 0;
					Struct* s = createStruct(current, &offset);
					structs->add(s);
					for (int i = 0; i < offset + 1; i++)
						current = tokens->child(current);
				}

				// Global Variable Declaration
				else if (t->getType() == VAR && 
						RTOKEN(1)->getType() == SPECIAL && RTOKEN(1)->getData() == COLON &&
						RTOKEN(2)->getType() == TYPE &&
						RTOKEN(3)->getType() == SPECIAL && RTOKEN(3)->getData() == EXCLAIM) {
					Variable* v = new Variable(names->get(t->getData()), types->get(RTOKEN(2)->getData()));
					// printf("var: {name=\"%s\", type=\"%s\"}\n", v->name, v->type->name);
					gvars->add(v);
					//t->setType(GVAR);

					char* buf = new char[256];
					sprintf(buf, VM_VAR_DEC_AND_INIT, v->name, getStoreSize(v), 0);
					this->varBuffer->add(strdup(buf));
					delete[] buf;
				}

				// TODO: FIX
				// Global Variable Declaration and Initialization
				else if (t->getType() == VAR && 
						RTOKEN(1)->getType() == SPECIAL && RTOKEN(1)->getData() == COLON &&
						RTOKEN(2)->getType() == TYPE &&
						RTOKEN(3)->getType() == OP && RTOKEN(3)->getData() == EQUALS &&
						RTOKEN(4)->getType() == EXPR) {
					// error("(%d): Global declaration and initialization is not supported yet\n", t->getLine());
					Variable* v = new Variable(names->get(t->getData()), types->get(RTOKEN(2)->getData()));
					gvars->add(v);
					//t->setType(GVAR);

					// Evaluate expression
					Expression* e = exprs->get(RTOKEN(4)->getData());
					List<char*>* lines = e->evaluate(out);
					for (int i = 0; i < lines->getSize(); i++)
						this->initBuffer->add(lines->get(i));
					char* buf = new char[256];
					sprintf(buf, VM_VAR_SET_E, getStoreSize(v), v->name, "eax");
					this->initBuffer->add(strdup(buf));
					delete[] buf;

					// Save variable declaration in data space
					buf = new char[256];
					sprintf(buf, VM_VAR_DEC_AND_INIT, v->name, getStoreSize(v), RTOKEN(4)->getData());
					this->varBuffer->add(strdup(buf));
					delete[] buf;
				}

				// Function
				else if (t->getType() == KEYWORD && t->getData() == FUNC &&
						RTOKEN(1)->getType() == ID) {
					current = tokens->child(current);
					Function* f = createFunction(current);
					currentFunction = f;
					fprintf(out, VM_LABEL, f->name);
				}

			} else {

				// printf("Inside of function\n");

				// Set Global Variable
				if (t->getType() == VAR && (temp = isGlobal(names->get(t->getData()))) != -1 &&
						RTOKEN(1)->getType() == OP && RTOKEN(1)->getData() == EQUALS &&
						RTOKEN(2)->getType() == EXPR) {
					Variable* v = gvars->get(temp);
					Expression* e = exprs->get(RTOKEN(2)->getData());
					List<char*>* lines = e->evaluate(out);
					for (int i = 0; i < lines->getSize(); i++)
						fprintf(out, "%s", lines->get(i));
					fprintf(out, VM_VAR_SET_E, getStoreSize(v), v->name, "eax");
					// TODO: Have evaluator keep track of registers
					goto end;
				}

				// End Function
				if (t->getType() == KEYWORD && t->getData() == ENDF) {
					fprintf(out, "\t%s\n\n", "ret");
					currentFunction = nullptr;
				}

				// Return Variable
				else if (t->getType() == KEYWORD && t->getData() == RETURN &&
						RTOKEN(1)->getType() == VAR) {
					temp = currentFunction->isParam(names->get(RTOKEN(1)->getData()));
					if (temp != -1) {
						Variable* v = currentFunction->params->get(temp);
						fprintf(out, VM_RETURN_P_VAR, getInstSize(v), currentFunction->getParamOffset(temp) + 8);
					} else if ((temp = isGlobal(names->get(RTOKEN(1)->getData()))) != -1) {
						Variable* v = gvars->get(temp);
						fprintf(out, VM_RETURN_G_VAR, getStoreSize(v), v->name);
					}
				}

				// Return Constant
				else if (t->getType() == KEYWORD && t->getData() == RETURN &&
						RTOKEN(1)->getType() == NUMBER) {
					fprintf(out, VM_RETURN_CONST, RTOKEN(1)->getData());
				}

				/*
				// Set Global Variable
				else if (t->getType() == VAR && (temp = isGlobal(names->get(t->getData()))) != -1 &&
						RTOKEN(1)->getType() == OP && RTOKEN(1)->getData() == EQUALS &&
						RTOKEN(2)->getType() == NUMBER) {
					Variable* v = gvars->get(temp);
					fprintf(out, VM_VAR_SET, getStoreSize(v), v->name, RTOKEN(2)->getData());
				}
				*/

				// Function Call
				else if (t->getType() == ID &&
						RTOKEN(-1)->getType() != KEYWORD &&
						RTOKEN(1)->getType() == SPECIAL && RTOKEN(1)->getData() == LEFT_PAR) {
					parseFunctionCall(current);
				}

			}

			end:
			current = tokens->child(current);
		}

		// Initializing Globals
		fprintf(out, "%s", "start:\n");
		for (int i = 0; i < initBuffer->getSize(); i++) {
			fprintf(out, "%s", initBuffer->get(i));
		}
		fprintf(out, "%s", VM_START_END);

		// Globals
		fprintf(out, "%s\n", "#section DATA");
		for (int i = 0; i < varBuffer->getSize(); i++) {
			fprintf(out, "%s", varBuffer->get(i));
		}

		// Ending File
		fprintf(out, "%s", "\n");
		fclose(out);
	}

	Function* Compiler::createFunction(LinkData<Token*>* func) {
		Function* f = new Function(ids->get(func->data->getData()));

		Token* t;
		LinkData<Token*>* current = func->child;
		while (current != nullptr) {
			t = current->data;

			// Parameter
			if (t->getType() == VAR &&
					RTOKEN(1)->getType() == SPECIAL && RTOKEN(1)->getData() == COLON &&
					RTOKEN(2)->getType() == TYPE) {
				char* name = names->get(t->getData());
				Type* type = types->get(RTOKEN(2)->getData());
				// printf("VAR: %s - %s\n", name, type->name);
				f->addParam(new Variable(name, type));
			}

			// Return Type
			else if (RTOKEN(-1)->getType() == SPECIAL && RTOKEN(-1)->getData() == LEFT_BRACK &&
					t->getType() == TYPE &&
					RTOKEN(1)->getType() == SPECIAL && RTOKEN(1)->getData() == RIGHT_BRACK) {
				Type* type = types->get(t->getData());
				// printf("RET: %s\n", type->name);
				f->setReturnType(type);
				break;
			}

			current = tokens->child(current);
		}

		funcs->add(f);
		return f;
	}

	Struct* Compiler::createStruct(LinkData<Token*>* start, int* offset) {
		Struct* s = new Struct(ids->get(start->data->getData()));
		// printf("Starting struct parse: %s\n", s->name);

		Token* t;
		LinkData<Token*>* current = tokens->child(start);
		int size = 0;
		while (current != nullptr) {
			t = current->data;

			// Variable
			if (t->getType() == VAR &&
					RTOKEN(1)->getType() == SPECIAL && RTOKEN(1)->getData() == COLON &&
					RTOKEN(2)->getType() == TYPE &&
					RTOKEN(3)->getType() == SPECIAL && RTOKEN(3)->getData() == EXCLAIM) {
				char* name = names->get(t->getData());
				Type* type = types->get(RTOKEN(2)->getData());
				Variable* v = new Variable(name, type);
				s->addVariable(v);
				// printf("\tvar: {name=\"%s\", type=\"%s\"}\n", v->name, v->type->name);
				size += type->size;
			}

			// End Struct
			else if (t->getType() == KEYWORD && t->getData() == END) {
				break;
			}

			current = tokens->child(current);
			*offset += 1;
		}

		s->size = size;
		this->types->add(new Type(s->name, s->size));
		// printf("Done parsing struct: {name=\"%s\", size=%d} (%d)\n", s->name, s->size, *offset);
		return s;
	}

	void Compiler::parseFunctionCall(LinkData<Token*>* func) {
		// Check Name
		char* fname = names->get(func->data->getData());
		Function* f;
		int n;
		if ((n = isFunction(fname)) != -1) {
			f = funcs->get(n);
		} else {
			error("(%d): No such function '%s'\n", func->data->getLine(), fname);
			exit(1);
		}

		// PUSHA
		fprintf(out, "%s", VM_CALL_HEADER);

		// Set up data
		int argb = 0;
		int argc = 0;
		List<Token*>* args = new List<Token*>(1);

		// Parse Arguments

		Token* t;
		LinkData<Token*>* current = func->child;
		while (current != nullptr) {
			t = current->data;

			// Variable
			if (t->getType() == VAR && RTOKEN(1)->getType() == SPECIAL &&
					(RTOKEN(1)->getData() == COMMA || RTOKEN(1)->getData() == RIGHT_PAR)) {
				int index;
				if ((index = isGlobal(names->get(t->getData()))) != -1) {
					t->setType(GVAR);
					t->setData(index);
				} else if ((index = currentFunction->isParam(names->get(t->getData()))) != -1) {
					t->setType(PVAR);
					t->setData(index);
				}
				args->add(t);
			}

			// Number
			else if (t->getType() == NUMBER && RTOKEN(1)->getType() == SPECIAL &&
					(RTOKEN(1)->getData() == COMMA || RTOKEN(1)->getData() == RIGHT_PAR)) {
				args->add(t);
			}

			// End of Call
			else if (t->getType() == SPECIAL && t->getData() == RIGHT_PAR) {
				break;
			}

			current = tokens->child(current);
		}

		// Check for argument count
		if (args->getSize() < f->params->getSize()) {
			error("(%d): Not enough arguments for '%s' (%d < %d)\n", func->data->getLine(), fname, args->getSize(), f->params->getSize());
			exit(1);
		} else if (args->getSize() > f->params->getSize()) {
			error("(%d): Too many arguments for '%s' (%d > %d)\n", func->data->getLine(), fname, args->getSize(), f->params->getSize());
			exit(1);
		}

		// Output to NVM

		for (int i = args->getSize() - 1; i >= 0; i--) {
			t = args->get(i);
			Variable* varg = f->params->get(i);

			switch (t->getType()) {
				case NUMBER: {
					fprintf(out, "\t%sconst %d\n", getInstSize(varg), t->getData());
					argb += varg->type->size;
					break;
				}

				case GVAR: {
					Variable* g = gvars->get(t->getData());
					if (g->type->size != varg->type->size) {
						error("(%d): Incorrect argument size\n", t->getLine());
						exit(1);
					}
					argb += g->type->size;
					fprintf(out, "\tld%s eax $g_%s\n", getStoreSize(g), g->name);
					fprintf(out, "\tstore eax\n");
					break;
				}

				case PVAR: {
					Variable* p = currentFunction->params->get(t->getData());
					if (p->type->size != varg->type->size) {
						error("(%d): Incorrect argument size\n", t->getLine());
						exit(1);
					}
					argb += p->type->size;
					fprintf(out, "\t%sget eax (ebp)+%d\n", getStoreSize(p), currentFunction->getParamOffset(t->getData()));
					fprintf(out, "\tstore eax\n");
					break;
				}

				default: {
					error("(%d): Invalid argument AST token %d\n", t->getLine(), t->getType());
					exit(1);
				}
			}
		}

		// Finish
		if (f->type == F_LOCAL) {
			fprintf(out, VM_CALL_END, fname, argb);
		} else if (f->type == F_NATIVE) {
			fprintf(out, VM_CALL_NATIVE, fname, argb);
		}
	}

	int Compiler::isFunction(char* name) {
		for (int i = 0; i < funcs->getSize(); i++) {
			if (!strcmp(name, funcs->get(i)->name)) {
				return i;
			}
		}
		return -1;
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
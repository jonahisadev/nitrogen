#include <Nitrogen/Compiler.h>

namespace Nitrogen {

	Compiler::Compiler() {
		out = fopen("out.nvm", "w");
		this->varBuffer = new List<char*>(1);
	}
	
	Compiler::~Compiler() {
		delete this->varBuffer;
	}
	
	void Compiler::start() {
		fprintf(out, "%s", VM_START);
		
		Token* t;
		
		for (int i = 0; i < tokens->getSize(); i++) {
			t = tokens->get(i);
			
			int temp;

			// FUNCTIONS

			if (currentFunction != nullptr) {

				// Return Parameter
				if (t->getType() == KEYWORD && t->getData() == RETURN &&
						 	(temp = currentFunction->isParam(names->get(tokens->get(i + 1)->getData()))) != -1) {
					int off = currentFunction->getParamOffset(temp);
					Variable *v = currentFunction->params->get(temp);
					fprintf(out, VM_RETURN_P_VAR, getInstSize(v), off + 8);
				}

				// Return Constant
				else if (t->getType() == KEYWORD && t->getData() == RETURN &&
						 tokens->get(i + 1)->getType() == NUMBER) {
					fprintf(out, VM_RETURN_CONST, tokens->get(i + 1)->getData());
				}

				// Return Global Variable
				else if (t->getType() == KEYWORD && t->getData() == RETURN &&
						 tokens->get(i + 1)->getType() == GVAR) {
					t = tokens->get(i + 1);
					fprintf(out, VM_RETURN_G_VAR, getStoreSize(t), gvars->get(t->getData())->name);
				}

				// End Function
				else if (t->getType() == KEYWORD && t->getData() == ENDF) {
					currentFunction = nullptr;
					fprintf(out, "\t%s\n\n", "ret");
				}

			}

			// EVERYTHING ELSE

			else {

				// Variable Declaration and Initialization
				if (t->getType() == GVAR &&
						tokens->get(i+1)->getType() == SPECIAL && tokens->get(i+1)->getData() == COLON &&
						tokens->get(i+2)->getType() == TYPE &&
						tokens->get(i+3)->getType() == SPECIAL && tokens->get(i+3)->getData() == EQUALS &&
						tokens->get(i+4)->getType() == NUMBER) {
					char* buf = new char[256];
					sprintf(buf, VM_VAR_DEC_AND_INIT, gvars->get(t->getData())->name, getStoreSize(t), tokens->get(i+4)->getData());
					this->varBuffer->add(buf);
				}

				// Global Variable Setting
				else if (t->getType() == GVAR &&
						 tokens->get(i + 1)->getType() == SPECIAL && tokens->get(i + 1)->getData() == EQUALS &&
						 tokens->get(i + 2)->getType() == NUMBER) {
					fprintf(out, VM_VAR_SET, getStoreSize(t), gvars->get(t->getData())->name, tokens->get(i + 2)->getData());
				}

				// Variable Declaration
				else if (t->getType() == GVAR &&
						tokens->get(i+1)->getType() == SPECIAL && tokens->get(i+1)->getData() == COLON &&
						tokens->get(i+2)->getType() == TYPE &&
						tokens->get(i+3)->getType() == SPECIAL && tokens->get(i+3)->getData() == EXCLAIM) {
					char* buf = new char[256];
					sprintf(buf, VM_VAR_DEC_AND_INIT, gvars->get(t->getData())->name, getStoreSize(t), 0);
					this->varBuffer->add(buf);
				}
				
				// Function
				else if (t->getType() == KEYWORD && t->getData() == FUNC) {
					Function* f = createFunction(t, &i);
					currentFunction = f;
					fprintf(out, VM_LABEL, f->name);
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

	Function* Compiler::createFunction(Token *tok, int* index) {
		Function* f = new Function(symbols->get(tokens->get(*index+1)->getData()));
		*index += 2;
		Token* t;
		for (int i = *index; i < tokens->getSize(); i++) {
			t = tokens->get(i);

			// Parameter
			if (t->getType() == PVAR) {
				char* name = strdup(names->get(t->getData()));
				Type* type = types->get(tokens->get(i+2)->getData());
				f->addParam(new Variable(name, type));
			}

			// Return Type
			else if (tokens->get(i-1)->getType() == SPECIAL && tokens->get(i-1)->getData() == LEFT_BRACK &&
					t->getType() == TYPE &&
					tokens->get(i+1)->getType() == SPECIAL && tokens->get(i+1)->getData() == RIGHT_BRACK) {
				Type* type = types->get(t->getData());
				f->setReturnType(type);
			}
		}
		return f;
	}

	const char* Compiler::getStoreSize(Token* var) {
		switch (gvars->get(var->getData())->type->size) {
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
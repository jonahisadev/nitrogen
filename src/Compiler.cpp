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
			
			// Variable Declaration and Initialization
			if (t->getType() == VAR &&
					tokens->get(i+1)->getType() == SPECIAL && tokens->get(i+1)->getData() == COLON &&
					tokens->get(i+2)->getType() == TYPE &&
					tokens->get(i+3)->getType() == SPECIAL && tokens->get(i+3)->getData() == EQUALS &&
					tokens->get(i+4)->getType() == NUMBER) {
				char* buf = new char[256];
				sprintf(buf, VM_VAR_DEC_AND_INIT, gvars->get(t->getData())->name, getStoreSize(t), tokens->get(i+4)->getData());
				this->varBuffer->add(buf);
			}
			
			// Function
			else if (t->getType() == KEYWORD && t->getData() == FUNC) {
				fprintf(out, VM_LABEL, symbols->get(tokens->get(i+1)->getData()));
			}
			
			// Return Constant
			else if (t->getType() == KEYWORD && t->getData() == RETURN &&
					tokens->get(i+1)->getType() == NUMBER) {
				fprintf(out, VM_RETURN_CONST, tokens->get(i+1)->getData());
			}
			
			// Return Global Variable
			else if (t->getType() == KEYWORD && t->getData() == RETURN &&
					tokens->get(i+1)->getType() == VAR) {
				t = tokens->get(i+1);
				fprintf(out, VM_RETURN_G_VAR, getStoreSize(t), gvars->get(t->getData())->name);
			}
			
			// End Function
			else if (t->getType() == KEYWORD && t->getData() == ENDF) {
				fprintf(out, "\t%s\n\n", "ret");
			}
		}
		
		fprintf(out, "%s\n", "#section DATA");
		for (int i = 0; i < varBuffer->getSize(); i++) {
			fprintf(out, "%s", varBuffer->get(i));
		}
		fprintf(out, "%s", "\n");
		fclose(out);
	}
	
	const char* Compiler::getStoreSize(Token* var) {
		switch (gvars->get(var->getData())->type->size) {
			case 4:
				return "dd";
			case 2:
				return "dw";
			default:
				return "dd";
		}
	}
	
	const char* Compiler::getLoadSize(Token* var) {
		switch (gvars->get(var->getData())->type->size) {
			case 4:
				return "ldd";
			case 2:
				return "ldw";
			default:
				return "ldd";
		}
	}

}
#include <Nitrogen/Compiler.h>

namespace Nitrogen {

	Compiler::Compiler() {
		out = fopen("out.nvm", "w");
		this->varBuffer = new List<char*>(1);
	}
	
	Compiler::~Compiler() {
		// Nothing
	}
	
	void Compiler::start() {
		fprintf(out, "%s", VM_START);
		
		Token* t;
		
		for (int i = 0; i < tokens->getSize(); i++) {
			t = tokens->get(i);
			
			// Variable Declaration and Initialization
			if (t->getType() == ID &&
					tokens->get(i+1)->getType() == SPECIAL && tokens->get(i+1)->getData() == COLON &&
					tokens->get(i+2)->getType() == NAME &&
					tokens->get(i+3)->getType() == SPECIAL && tokens->get(i+3)->getData() == EQUALS &&
					tokens->get(i+4)->getType() == NUMBER) {
				char* buf = new char[256];
				sprintf(buf, VM_VAR_DEC_AND_INIT, symbols->get(t->getData()), "dd", tokens->get(i+4)->getData());
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
					tokens->get(i+1)->getType() == NAME) {
				fprintf(out, VM_RETURN_G_VAR, "dd", names->get(tokens->get(i+1)->getData()));
			}
			
			// End Function
			else if (t->getType() == KEYWORD && t->getData() == ENDF) {
				fprintf(out, "\t%s\n\n", "ret");
			}
		}
		
		fprintf(out, "%s\n", "#section DATA");
		for (int i = 0; i < varBuffer->getSize(); i++) {
			fprintf(out, "%s\n", varBuffer->get(i));
		}
		fclose(out);
	}

}
#include <Nitrogen/Compiler.h>

namespace Nitrogen {

	Compiler::Compiler() {
		out = fopen("out.nvm", "w");
	}
	
	Compiler::~Compiler() {
		// Nothing
	}
	
	void Compiler::start() {
		fprintf(out, "%s", VM_START);
		
		Token* t;
		
		for (int i = 0; i < tokens->getSize(); i++) {
			t = tokens->get(i);
			
			// Function
			if (t->getType() == KEYWORD && t->getData() == FUNC) {
				fprintf(out, VM_LABEL, symbols->get(tokens->get(i+1)->getData()));
			}
			
			// Return Constant
			if (t->getType() == KEYWORD && t->getData() == RETURN &&
					tokens->get(i+1)->getType() == NUMBER) {
				fprintf(out, VM_RETURN_CONST, tokens->get(i+1)->getData());
			}
			
			// End Function
			if (t->getType() == KEYWORD && t->getData() == ENDF) {
				fprintf(out, "\t%s\n\n", "ret");
			}
		}
		
		fclose(out);
	}

}
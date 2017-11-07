#include <Nitrogen/Parser.h>

namespace Nitrogen {

	Parser::Parser(char* source, int length) {
		this->source = source;
		this->length = length;
	}
	
	Parser::~Parser() {
		delete[] this->source;
	}
	
	void Parser::start() {
		printf("%s\n", this->source);
	}

}
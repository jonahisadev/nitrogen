#include <Nitrogen/Parser.h>

namespace Nitrogen {

	Parser::Parser(char* source, int length) {
		this->source = source;
		this->length = length;
		
		this->tokens = new List<Token*>(1);
		this->names = new List<char*>(1);
	}
	
	Parser::~Parser() {
		delete[] this->source;
	}
	
	void Parser::start() {
		char* lex = new char[256];
		int lexi = 0;
		int i = 0;
		int line = 1;
		
		resetLex:
		for (int x = 0; x < 256; x++)
			lex[x] = '\0';
		lexi = 0;
		
		while (!isImportant(source[i])) {
			if (source[i] == '\0')
				break;
			if (source[i] == '\n')
				break;
			if (source[i] == '\t') {
				i++;
				goto resetLex;
			}
			
			lex[lexi++] = source[i++];
		}
		
		lex[lexi] = '\0';
		
		printf("(%d) ", line);
		
		if (lexi == 0) {
			printf("special: %c\n", source[i]);
			
			switch (source[i]) {
				case '(': {
					tokens->add(new Token(SPECIAL, LEFT_PAR, line));
					break;
				}
				case ')': {
					tokens->add(new Token(SPECIAL, RIGHT_PAR, line));
					break;
				}
				case '[': {
					tokens->add(new Token(SPECIAL, LEFT_BRACK, line));
					break;
				}
				case ']': {
					tokens->add(new Token(SPECIAL, RIGHT_BRACK, line));
					break;
				}
				case ':': {
					tokens->add(new Token(SPECIAL, COLON, line));
					break;
				}
				default: {
					printf("ERR: (%d) Invalid special character\n", line);
					exit(1);
				}
			}
			
			i++;
			goto end;
		}
		
		// printf("%s\n", lex);
		
		int token;
		
		if ((token = isKeyword(lex)) != -1) {
			printf("key: %s\n", lex);
			tokens->add(new Token(KEYWORD, token, line));
			goto end;
		} 
		
		else if (Util::isNumber(lex)) {
			printf("num: %d\n", Util::toNum(lex, 10));
			tokens->add(new Token(NUMBER, Util::toNum(lex, 10), line));
			goto end;
		}
		
		else {
			printf("name: %s\n", lex);
			names->add(strdup(lex));
			tokens->add(new Token(NAME, names->getSize()-1, line));
			goto end;
		}
		
		end:
		if (i >= length)
			return;
		if (source[i] == '\0')
			return;
		else if (source[i] == '\n') {
			i++;
			line++;
		} else if (source[i] == ' ') {
			i++;
		}
		goto resetLex;
	}
	
	int Parser::isKeyword(char* str) {
		if (!strcmp(str, "func"))
			return TokenKeyword::FUNC;
		else if (!strcmp(str, "return"))
			return TokenKeyword::RETURN;
		else if (!strcmp(str, "endf"))
			return TokenKeyword::ENDF;
		else
			return -1;
	}
	
	bool Parser::isImportant(char c) {
		switch (c) {
			case '(':
				return true;
			case ')':
				return true;
			case '[':
				return true;
			case ']':
				return true;
			case ':':
				return true;
			case ' ':
				return true;
			default:
				return false;
		}
	}

}
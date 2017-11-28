#include <Nitrogen/Parser.h>

namespace Nitrogen {

	Parser::Parser(char* source, int length) {
		this->source = source;
		this->length = length;
		
		this->tokens = new List<Token*>(1);
		this->names = new List<char*>(1);
		
		this->types = new List<Type*>(1);
		types->add(new Type("i32", 4));
		types->add(new Type("i16", 2));
		types->add(new Type("i8", 1));
		types->add(new Type("void", 0));
		types->add(new Type("string", -1));
	}
	
	Parser::~Parser() {
		delete[] this->source;
		delete this->types;
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
		
		if (lexi == 0) {
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
				case '=': {
					tokens->add(new Token(SPECIAL, EQUALS, line));
					break;
				}
				case ',': {
					tokens->add(new Token(SPECIAL, COMMA, line));
					break;
				}
				case '!': {
					tokens->add(new Token(SPECIAL, EXCLAIM, line));
					break;
				}
				case '\n': {
					goto end;
				}
				default: {
					printf("ERR: (%d) Invalid special character\n", line);
					exit(1);
				}
			}
			
			// printf("(%d) ", line);
			// printf("special: %c\n", source[i]);
			i++;
			goto end;
		}
		
		// printf("(%d) ", line);
		
		// printf("%s\n", lex);
		
		int token;
		
		if ((token = isKeyword(lex)) != -1) {
			// printf("key: %s\n", lex);
			tokens->add(new Token(KEYWORD, token, line));
			goto end;
		} 
		
		else if (Util::isNumber(lex)) {
			// printf("num: %d\n", Util::toNum(lex, 10));
			tokens->add(new Token(NUMBER, Util::toNum(lex, 10), line));
			goto end;
		}
		
		else if ((token = isType(lex)) != -1) {
			// printf("type: %d\n", token);
			tokens->add(new Token(TYPE, token, line));
			goto end;
		}
		
		else {
			// printf("name: %s\n", lex);
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
	
	int Parser::isType(char* str) {
		for (int i = 0; i < types->getSize(); i++) {
			if (!strcmp(str, types->get(i)->name)) {
				return i;
			}
		}
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
			case '=':
				return true;
			case ',':
				return true;
			case '!':
				return true;
			default:
				return false;
		}
	}
	
	Context* Parser::createContext() {
		Context* c = new Context();
		c->setTokens(this->tokens);
		c->setNames(this->names);
		c->setTypes(this->types);
		return c;
	}

}
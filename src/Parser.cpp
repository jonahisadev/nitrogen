#include <Nitrogen/Parser.h>

namespace Nitrogen {

	Parser::Parser(char* source, int length) {
		this->source = source;
		this->length = length;
		
		this->tokens = new LinkedList<Token*>(nullptr);
		this->names = new List<char*>(1);
		
		this->types = new List<Type*>(1);
		types->add(new Type("i32", 4, true));
		types->add(new Type("i16", 2, true));
		types->add(new Type("i8", 1, true));
		types->add(new Type("void", 0, true));
		types->add(new Type("string", 4, true));		// 4 because pointer
	}
	
	Parser::~Parser() {
		delete[] this->source;
		delete this->types;
	}
	
	void Parser::start() {
		char lex[256];
		int lexi = 0;
		int i = 0;
		int line = 1;
		
		resetLex:
		for (int x = 0; x < 256; x++)
			lex[x] = '\0';
		lexi = 0;
		
		while (!isImportant(source[i])) {
			if (comment) {
				if (source[i] == '\n') {
					this->comment = false;
					goto resetLex;
				}
				i++;
				continue;
			}

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
		
		// SPECIAL
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
					tokens->add(new Token(OP, EQUALS, line));
					break;
				}
				case '+': {
					tokens->add(new Token(OP, PLUS, line));
					break;
				}
				case '-': {
					tokens->add(new Token(OP, MINUS, line));
					break;
				}
				case '*': {
					tokens->add(new Token(SPECIAL, STAR, line));
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
				case '#': {
					this->comment = true;
					// printf("Found a comment (%d)\n", line);
					break;
				}
				case '$': {
					tokens->add(new Token(KEYWORD, DOLLAR, line));
					break;
				}
				case '.': {
					tokens->add(new Token(SPECIAL, DOT, line));
					break;
				}
				case ' ': {
					goto end;
				}
				case '\n': {
					goto end;
				}
				default: {
					error("(%d): Invalid special character '%c'\n", line, source[i]);
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
		
		// KEYWORD
		if ((token = isKeyword(lex)) != -1) {
			// printf("key: %s\n", lex);
			tokens->add(new Token(KEYWORD, token, line));
			goto end;
		} 
		
		// NUMBER
		else if (Util::isNumber(lex)) {
			// printf("num: %d\n", Util::toNum(lex, 10));
			tokens->add(new Token(NUMBER, Util::toNum(lex, 10), line));
			goto end;
		}
		
		// TYPE
		else if ((token = isType(lex)) != -1) {
			// printf("type: %d\n", token);
			tokens->add(new Token(TYPE, token, line));
			goto end;
		}
		
		// OTHER
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
		else if (!strcmp(str, "native"))
			return TokenKeyword::NATIVE;
		else if (!strcmp(str, "struct"))
			return TokenKeyword::STRUCT;
		else if (!strcmp(str, "end"))
			return TokenKeyword::END;
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
				return true;
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
			case '.':
				return true;
			case '+':
				return true;
			case '-':
				return true;
			case '*':
				return true;
			case '#':
				return true;
			case '$':
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
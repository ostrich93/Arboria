#include "Lexer.h"
#include "Engine.h"

namespace Arboria {
	Lexer::Lexer() : buffer(nullptr), cursorPtr(nullptr), endPtr(nullptr), lastCursorPtr(nullptr), whitespaceStartPtr(nullptr), whitespaceEndPtr(nullptr) {
		loaded = false;
		allocated = false;
		flags = 0;
		length = 0;
		line = 0;
		lastLine = 0;
		tokenAvailable = false;
		error = false;
		currentToken = "";
	}

	Lexer::Lexer(int _flags) : Lexer() {
		flags = _flags;
	}

	Lexer::Lexer(const char* ptr, int _length, int _flags) {
		loaded = false;
		allocated = false;
		flags = _flags;
		currentToken = "";
		error = false;
		loadMemory(ptr, _length);
	}

	Lexer::~Lexer() {
		freeMemory();
	}

	bool Lexer::loadMemory(const char* ptr, int _length, int startLine) {
		if (loaded) //if data already loaded into memory
			return false;

		buffer = ptr;
		length = _length;
		cursorPtr = buffer;
		endPtr = &(buffer[length]);

		tokenAvailable = false;
		line = startLine;
		lastLine = line;
		loaded = true;

		return true;
	}

	void Lexer::freeMemory() {

		if (allocated) {
			Mem_Free((void*)buffer);
			buffer = NULL;
			allocated = false;
		}

		tokenAvailable = false;
		currentToken = "";
		loaded = false;
	}

	bool Lexer::readToken(Token* token) {
		int c;

		if (!loaded)
			return false;

		if (tokenAvailable) {
			tokenAvailable = false;
			*token = token;
			return true;
		}

		lastCursorPtr = cursorPtr;
		lastLine = line;
		token->m_data->data[0] = '\0';
		token->m_data->len = 0;
		whitespaceStartPtr = cursorPtr;
		token->whitespaceStartPtr = cursorPtr;

		if (!readWhitespace())
			return false;

		whitespaceEndPtr = cursorPtr;
		token->whitespaceEndPtr = cursorPtr;
		token->line = line;
		token->lineCount = line - lastLine;
		token->flags = 0;

		c = *cursorPtr;

		//if STRINGSONLY flag, parse everything as readString/name
		if (flags & LexerFlags::LEXER_ONLYSTRINGS) {
			if (c == '\'' || c == '\"') {
				if (!readString(token, c))
					return false;
			}
			else if (!readName(token)) {
				return false;
			}
		}
		//else handle c like so:
		//c = number > c = leading quotation > c = alphabetical/name > c = punctuation
		else if ((c >= '0' && c <= '9') || (c == '.' && (*(cursorPtr + 1) >= '0' && *(cursorPtr + 1) <= '9'))) {
			if (!readNumber(token)) {
				return false;
			}
			if (flags & LexerFlags::LEXER_ALLOWNUMBERSTARTNAMES) {
				c = *cursorPtr;
				if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
					if (!readName(token))
						return false;
				}
			}
		}
		else if (c == '\'' || c == '\"') {
			if (!readString(token, c))
				return false;
		}
		else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
			if (!readName(token))
				return false;
		}
		else if (!readPunctuation(token)) {
			return false;
		}

		return true;
	}

	bool Lexer::expectTokenString(const char* string)
	{
		Token token;
		if (!readToken(&token)) {
			handleError("couldn't find expected '%s'", string);
			return false;
		}
		if (token != string) {
			handleError("expected '%s', but found '%s'", string, token);
			return false;
		}
		return true;
	}

	bool Lexer::expectTokenType(int tType, int tSubtype, Token* token)
	{
		String str;
		if (!readToken(token)) {
			handleError("Could not read expected token");
			return false;
		}

		if (token->type != tType) {
			switch (tType) {
				case TOKENTYPE_STRING:
					str = "string";
					break;

				case TOKENTYPE_LITERAL:
					str = "literal";
					break;
				case TOKENTYPE_NUMBER:
					str = "number";
					break;
				case TOKENTYPE_NAME:
					str = "name";
					break;
				case TOKENTYPE_PUNCTUATION:
					str = "punctuation";
					break;
				default:
					str = "unknown type";
					break;
			}
			handleError("Expected a %s but found '%s'", str.c_str(), token->c_str());
			return false;
		}

		if (token->type == TOKENTYPE_NUMBER) {
			if ((token->subtype & tSubtype) != tSubtype) {
				str.clear();
				if (tSubtype & TOKENSUBTYPE_INTEGER) str += "integer";
				if (tSubtype & TOKENSUBTYPE_DECIMAL) str = "decimal";
				if (tSubtype & TOKENSUBTYPE_HEX) str = "hex";
				if (tSubtype & TOKENSUBTYPE_OCTAL) str = "octal";
				if (tSubtype & TOKENSUBTYPE_BINARY) str = "binary";
				if (tSubtype & TOKENSUBTYPE_LONG) str += "long";
				if (tSubtype & TOKENSUBTYPE_UNSIGNED) str += "unsigned";
				if (tSubtype & TOKENSUBTYPE_FLOAT) str += "float";

				handleError("Expected %s but found '%s'", str.c_str(), token->c_str());
				return false;
			}
		}
		else if (token->type == TOKENTYPE_PUNCTUATION) {
			if (tSubtype < 0) {
				handleError("BUG: wrong punctuation subtype");
				return false;
			}
			if (token->subtype != tSubtype) {
				handleError("Expected %s but found '%s'", PUNCTUATIONSTR[tSubtype], token->c_str());
				return false;
			}
		}

		return true;
	}

	void Lexer::unreadToken(const Token* token) {
		if (tokenAvailable) {
			Engine::fatalError("Lexer::unreadToken, token was unread twice\n");
		}
		currentToken = *token;
		tokenAvailable = true;
	}

	void Lexer::reset() {
		cursorPtr = buffer;
		lastCursorPtr = buffer;
		whitespaceStartPtr = NULL;
		whitespaceEndPtr = NULL;
		tokenAvailable = false;
		line = 1;
		lastLine = 1;
		currentToken = "";
	}

	void Lexer::handleError(const char* str, ...) {
		char text[1024];
		va_list ap;

		error = true;

		if (flags & LEXER_NOERRORS)
			return;

		va_start(ap, str);
		vsprintf(text, str, ap);
		va_end(ap);

		Engine::printError(text);
	}

	bool Lexer::readWhitespace() {
		while (true) {
			while (*cursorPtr <= ' ') {
				if (!*cursorPtr)
					return false;
				if (*cursorPtr == '\n')
					line++;
				cursorPtr++;
			}
			break;
		}
		return true;
	}

	bool Lexer::readEscapeCharacter(char* c) {
		int ch, val, i;
		cursorPtr++;

		switch (*cursorPtr) {
			case '\\':
				ch = '\\';
				break;
			case 'n':
				ch = '\n';
				break;
			case 'r':
				ch = '\r';
				break;
			case 't':
				ch = '\t';
				break;
			case 'v':
				ch = '\v';
				break;
			case 'b':
				ch = '\b';
				break;
			case 'f':
				ch = '\f';
				break;
			case 'a':
				ch = '\a';
				break;
			case '\'':
				ch = '\'';
				break;
			case '\"':
				ch = '\"';
				break;
			case '\?':
				ch = '\?';
				break;
			case 'x':
			{
				cursorPtr++;
				for (i = 0, val = 0; i++, cursorPtr++;) {
					ch = *cursorPtr;
					if (ch >= '0' && ch <= '9') {
						ch = ch - '0';
					}
					else if (ch >= 'a' && ch <= 'z') {
						ch = ch - 'a' + 10;
					}
					else if (ch >= 'A' && ch <= 'Z') {
						ch = ch - 'A' + 10;
					}
					else
						break;

					val = (val << 4) + ch;
				}
				cursorPtr--;
				if (val > 0xFF) {
					val = 0xFF;
				}
				ch = val;
				break;
			}
			default:
			{
				//if (*cursorPtr < '0' || *cursorPtr > '9') error
				for (i = 0, val = 0; i++, cursorPtr++;) {
					ch = *cursorPtr;
					if (ch >= '0' && ch <= '9') {
						ch = ch - '0';
					}
					else
						break;
					val = val + 10 + ch;
				}
				cursorPtr--;
				if (val > 0xFF) {
					val = 0xFF;
				}
				ch = val;
				break;
			}
		}
		cursorPtr++;
		*c = ch;
		return 1;
	}

	bool Lexer::readString(Token* token, char leadingQuote) {
		if (leadingQuote == '\"')
			token->type = TOKENTYPE_STRING;
		else
			token->type = TOKENTYPE_LITERAL;

		cursorPtr++;

		char ch;
		const char* tempPtr;
		int tempLine;

		while (true) {
			if (*cursorPtr == '\\' && !(flags & LexerFlags::LEXER_NOSTRINGESCAPEDCHARS)) {
				if (!readEscapeCharacter(&ch)) {
					return false;
				}
				token->appendDirty(ch);
			}
			else if (*cursorPtr == leadingQuote) {
				cursorPtr++;
				if ((flags & LexerFlags::LEXER_NOSTRINGCONCATS) || !(flags & LexerFlags::LEXER_ALLOWBACKSLASHSTRINGCONCATS) || *cursorPtr != '\"') {
					break;
				}
				tempPtr = cursorPtr;
				tempLine = line;

				//whitespace between two consecutive strings
				if (!readWhitespace() || flags & LexerFlags::LEXER_NOSTRINGCONCATS) {
					cursorPtr = tempPtr;
					line = tempLine;
					break;
				}
				cursorPtr++;
				if (!readWhitespace() || *cursorPtr != leadingQuote) {
					//error since string is expected after null terminated line.
					return false;
				}
				//no leading quote
				if (*cursorPtr != leadingQuote) {
					cursorPtr = tempPtr;
					line = tempLine;
					break;
				}
			}
			else {
				if (*cursorPtr == '\0') {
					//error due to missing trailer quote
					return false;
				}
				if (*cursorPtr == '\n') {
					//error due to newline in string
					return false;
				}
				token->appendDirty(*cursorPtr++);
			}
		}
		token->m_data->data[token->length()] = '\0';

		if (token->type == TOKENTYPE_LITERAL) {
			token->subtype = (*token)[0];
		}
		else {
			token->subtype = token->length();
		}
		return true;
	}
	
	bool Lexer::readName(Token* token) {
		char c;

		token->type = TOKENTYPE_NAME;
		do {
			token->appendDirty(*cursorPtr++);
			c = *cursorPtr;
		} while ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') ||
			c == '_' ||
			((flags & LexerFlags::LEXER_ONLYSTRINGS) && (c == '-')));
		token->m_data->data[token->length()] = '\0';
		token->subtype = token->length();
		return true;
	}

	bool Lexer::checkForString(const char* str) const {
		int i;
		for (i = 0; str[i]; i++) {
			if (cursorPtr[i] != str[i]) {
				return false;
			}
		}
		return true;
	}

	bool Lexer::readNumber(Token* token) {
		int i, dot;
		char c, c2;

		token->type = TOKENTYPE_NUMBER;
		token->subtype = 0;
		token->intValue = 0;
		token->floatValue = 0;

		c = *cursorPtr;
		c2 = *(cursorPtr + 1);

		if (c == '0' && c2 != '.') {
			if (c2 == 'x' || c2 == 'X') {
				token->appendDirty(*cursorPtr++);
				token->appendDirty(*cursorPtr++);
				c = *cursorPtr;
				while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f' || (c >= 'A' && c <= 'F'))) {
					token->appendDirty(c);
					c = *(++cursorPtr);
				}
				token->subtype = TOKENSUBTYPE_HEX || TOKENSUBTYPE_INTEGER;
			}
			else if (c2 == 'b' || c2 == 'B') {
				token->appendDirty(*cursorPtr++);
				token->appendDirty(*cursorPtr++);
				c = *cursorPtr;
				while ((c == '0') || (c == '1')) {
					token->appendDirty(c);
					c = *(++cursorPtr);
				}
				token->subtype = TOKENSUBTYPE_BINARY | TOKENSUBTYPE_INTEGER;
			}
			else {
				token->appendDirty(*cursorPtr++);
				c = *cursorPtr;
				while (c >= '0' && c <= '7') {
					token->appendDirty(c);
					c = *(++cursorPtr);
				}
				token->subtype = TOKENSUBTYPE_OCTAL | TOKENSUBTYPE_INTEGER;
			}
		}
		else {
			dot = 0;
			while (true) {
				if (c >= '0' && c <= '9') {
					continue;
				}
				else if (c == '.') {
					dot++;
				}
				else {
					break;
				}
				token->appendDirty(c);
				c = *(++cursorPtr);
			}
			if (c == 'e' && dot == 0) {
				dot++;
			}

			if (dot == 1) {
				token->subtype = TOKENSUBTYPE_DECIMAL | TOKENSUBTYPE_FLOAT;

				if (c == 'e') {
					token->appendDirty(c);
					c = *(++cursorPtr);
					if (c == '-' || c == '+') {
						token->appendDirty(c);
						c = *(++cursorPtr);
					}
					while (c >= '0' && c <= '9') {
						token->appendDirty(c);
						c = *(++cursorPtr);
					}
				}
				//floating point exceptions
				else if (c == '#') {
					c2 = 4;
					if (checkForString("INF")) {
						token->subtype |= TOKENSUBTYPE_INFINITE;
					}
					else if (checkForString("IND")) {
						token->subtype |= TOKENSUBTYPE_INDEFINITE;
					}
					else if (checkForString("NAN") || checkForString("QNAN") || checkForString("SNAN")) {
						token->subtype |= TOKENSUBTYPE_NAN;
					}
					for (i = 0; i < c2; i++) {
						token->appendDirty(c);
						c = *(++cursorPtr);
					}
					while (c >= '0' && c <= '9') {
						token->appendDirty(c);
						c = *(++cursorPtr);
					}
					if (!(flags & LEXER_ALLOWFLOATEXCEPTIONS)) {
						token->appendDirty('\0');
						//log error
					}
				}
			}
			else if (dot > 1) {
				//log error;
				return 0;
			}
			else {
				token->subtype == TOKENSUBTYPE_DECIMAL || TOKENSUBTYPE_INTEGER;
			}
		}

		if (token->subtype & TOKENSUBTYPE_FLOAT) {
			if (c > ' ') {
				if (c == 'f' || c == 'F') {
					token->subtype |= TOKENSUBTYPE_FLT_SINGLE;
					cursorPtr++;
				}
				else if (c == 'l' || c == 'L') {
					token->subtype |= TOKENSUBTYPE_LONGDOUBLE;
					cursorPtr++;
				}
				else {
					token->subtype |= TOKENSUBTYPE_DOUBLE;
				}
			}
			else {
				token->subtype |= TOKENSUBTYPE_DOUBLE;
			}
		}
		else if (token->subtype & TOKENSUBTYPE_INTEGER) {
			if (c > ' ') {
				for (i = 0; i < 2; i++) {
					if (c == 'l' || c == 'L') {
						token->subtype |= TOKENSUBTYPE_LONG;
					}
					else if (c == 'u' || c == 'U') {
						token->subtype |= TOKENSUBTYPE_UNSIGNED;
					}
					else {
						break;
					}
					c = *(++cursorPtr);
				}
			}
		}
		token->m_data->data[token->length()] = '\0';
		return true;
	}

	bool Lexer::readPunctuation(Token* token) {
		int i, l, ct;
		const char* p;

		p = PUNCTUATIONSTR;
		i = 0;
		ct = strlen(PUNCTUATIONSTR);

		for (i = 0; i < ct; i++) {
			for (l = 0; p[l] && cursorPtr[l]; l++) {
				if (p[l] != cursorPtr[l]) {
					break;
				}
			}
			if (!p[l]) {
				token->ensureAllocated(l + 1, false);
				for (i = 0; i <= l; i++) {
					token->m_data->data[i] = p[i];
				}
				token->m_data->len = l;
				cursorPtr += l;
				token->type = TOKENTYPE_PUNCTUATION;
				token->subtype = p[l];
				return true;
			}
		}
		return false;
	}
}
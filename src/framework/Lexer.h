#ifndef LEXER_H
#define LEXER_H

#include "Token.h"

namespace Arboria {

	const char PUNCTUATIONSTR[] = "!@#$%^&*()+-=|<>[]{},;:\\";

	enum LexerFlags {
		LEXER_NOERRORS = 1 << 0,
		LEXER_NOWARNINGS = 1 << 1,
		LEXER_NOFATALERRORS = 1 << 2,
		LEXER_NOSTRINGCONCATS = 1 << 3,
		LEXER_NOSTRINGESCAPEDCHARS = 1 << 4,
		LEXER_ALLOWNUMBERSTARTNAMES = 1 << 5,
		LEXER_ALLOWFLOATEXCEPTIONS = 1 << 6,
		LEXER_ALLOWBACKSLASHSTRINGCONCATS = 1 << 7,
		LEXER_ONLYSTRINGS = 1 << 8
	};

	class Token;
	class Lexer {
		public:
			Lexer();
			Lexer(int _flags);
			Lexer(const char* ptr, int length, int _flags = 0);
			~Lexer();

			bool loadMemory(const char* ptr, int _length, int startLine = 1);
			void freeMemory();
			bool isLoaded();
			bool readToken(Token* token);
			void unreadToken(const Token* token);
			void setFlags(int _flags);
			int getFlags();
			void reset();
			const int getLineNumber();
			bool hasError() const;
			bool atEnd();
		private:
			bool loaded;
			bool allocated;
			const char* buffer;
			const char* cursorPtr;
			const char* endPtr;
			const char* lastCursorPtr;
			const char* whitespaceStartPtr;
			const char* whitespaceEndPtr;
			int length;
			int line;
			int lastLine;
			bool tokenAvailable;
			int flags;
			Token currentToken;
			bool error;
		private:
			bool readWhitespace();
			bool readEscapeCharacter(char *c);
			bool readString(Token* token, char leadingQuote);
			bool readName(Token* token);
			bool readNumber(Token* token);
			bool readPunctuation(Token* token);
			bool checkForString(const char* str) const; //used for determining INF, IND, etc.
	};

	inline bool Lexer::isLoaded() { return loaded; }
	inline void Lexer::setFlags(int _flags) { flags = _flags; }
	inline int Lexer::getFlags() { return flags; }
	inline const int Lexer::getLineNumber() { return line; }
	inline bool Lexer::hasError() const { return error; }
	inline bool Lexer::atEnd() { return cursorPtr >= endPtr; }
}

#endif
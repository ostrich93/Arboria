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

	struct PunctuationEntry {
		const char* p;
	};

	class Token;
	class Lexer {
		public:
			Lexer();
			Lexer(int _flags);
			Lexer(const char* _filename, int flags = 0);
			Lexer(const char* ptr, int length, const char* name, int _flags = 0);
			~Lexer();

			int loadFile(const char* _filename);
			bool loadMemory(const char* ptr, int _length, const char* name, int startLine = 1);
			void freeMemory();
			bool isLoaded();
			bool readToken(Token* token);
			bool expectTokenString(const char* string);
			bool expectTokenType(int tType, int tSubtype, Token* token);
			bool expectAnyToken(Token* token);
			bool checkTokenString(const char* string);
			bool checkTokenType(int _type, int _subtype, Token* token);
			void unreadToken(const Token* token);
			void setFlags(int _flags);
			int getFlags();
			void reset();
			const int getLineNumber();
			void handleError(const char* str, ...);
			bool hasError() const;
			bool atEnd();
			float parseFloat();
			bool parseBool();
			int parseInt();
		private:
			bool loaded;
			String filename;
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
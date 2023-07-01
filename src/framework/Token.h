#ifndef TOKEN_H
#define TOKEN_H

#include "String.h"

namespace Arboria {

	#define TOKENTYPE_STRING		1
	#define TOKENTYPE_LITERAL		2
	#define TOKENTYPE_NUMBER		3
	#define TOKENTYPE_NAME			4
	#define TOKENTYPE_PUNCTUATION	5

	#define TOKENSUBTYPE_INTEGER	0x00001
	#define TOKENSUBTYPE_DECIMAL	0x00002
	#define TOKENSUBTYPE_HEX		0x00004
	#define TOKENSUBTYPE_OCTAL		0x00008
	#define TOKENSUBTYPE_BINARY		0x00010
	#define TOKENSUBTYPE_LONG		0x00020
	#define TOKENSUBTYPE_UNSIGNED	0x00040
	#define TOKENSUBTYPE_FLOAT		0x00080
	#define TOKENSUBTYPE_FLT_SINGLE 0x00100
	#define TOKENSUBTYPE_DOUBLE		0x00200
	#define TOKENSUBTYPE_LONGDOUBLE 0x00400
	#define TOKENSUBTYPE_INFINITE	0x00800
	#define	TOKENSUBTYPE_INDEFINITE	0x01000
	#define TOKENSUBTYPE_NAN		0x02000
	#define TOKENSUBTYPE_VALID		0x10000

	class Token : public String {
		friend class Lexer;
		public:
			unsigned char type;
			int subtype;
			int line;
			int lineCount;
			int flags;
		public:
			Token() = default;
			Token(const Token* token);
			~Token() = default;

			void operator=(const String& text);
			void operator=(const char* text);

			double getDoubleValue();
			float getFloatValue();
			int getIntegerValue();
			unsigned int getUnsignedIntegerValue();
			bool isWhitespaceBeforeToken() const;
			void clearTokenWhitespace();

			void calculateNumberValue();

		private:
			unsigned int intValue;
			double floatValue;
			const char* whitespaceStartPtr;
			const char* whitespaceEndPtr;
			Token* next;

			void appendDirty(const char c); //add a character without the trailing zero
	};

	inline Token::Token(const Token* token) {
		*this = *token;
	}

	inline void Token::operator=(const String& text) {
		*static_cast<String*>(this) = text;
	}

	inline void Token::operator=(const char* text) {
		*static_cast<String*>(this) = text;
	}

	inline double Token::getDoubleValue() {
		if (type != TOKENTYPE_NUMBER) {
			return 0.0;
		}
		if (!(subtype & TOKENSUBTYPE_VALID)) {
			
		}

		return floatValue;
	}

	inline float Token::getFloatValue() {
		return (float)getDoubleValue();
	}

	inline unsigned int Token::getUnsignedIntegerValue() {
		if (type != TOKENTYPE_NUMBER)
			return 0;
		if (!(subtype & TOKENSUBTYPE_VALID))
			calculateNumberValue();

		return intValue;
	}

	inline int Token::getIntegerValue() {
		return (int)getUnsignedIntegerValue();
	}

	inline bool Token::isWhitespaceBeforeToken() const {
		return (whitespaceEndPtr > whitespaceStartPtr);
	}

	inline void Token::appendDirty(const char c) {
		ensureAllocated(m_data->len + 2, true);
		m_data->data[m_data->len] = c;
	}
}

#endif
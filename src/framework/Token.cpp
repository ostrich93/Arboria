#include "Token.h"
#include "../utils/math.h"

namespace Arboria {
	void Token::clearTokenWhitespace() {
		whitespaceStartPtr = NULL;
		whitespaceEndPtr = NULL;
		lineCount = 0;
	}

	void Token::calculateNumberValue() {
		double m;
		bool div;
		int pow, i;
		const char* p;
		assert(type == TOKENTYPE_NUMBER);

		p = c_str();
		floatValue = 0.0;
		intValue = 0;

		if (subtype & TOKENSUBTYPE_FLOAT) {
			if (subtype & (TOKENSUBTYPE_INDEFINITE | TOKENSUBTYPE_INFINITE | TOKENSUBTYPE_NAN)) {
				if (subtype & TOKENSUBTYPE_INFINITE) {
					unsigned int inf = 0x7f800000;
					floatValue = (double)*(float*)&inf;
				}
				else if (subtype & TOKENSUBTYPE_INDEFINITE) {
					unsigned int ind = 0xffc00000;
					floatValue = (double)*(float*)&ind;
				}
				else if (subtype & TOKENSUBTYPE_NAN) {
					unsigned int nan = 0x7fc00000;
					floatValue = (double)*(float*)&nan;
				}
			}
			else {
				while (*p && *p != '.' && *p != 'e') {
					floatValue = floatValue * 10.0 + (double)(*p - '0');
					p++;
				}
				if (*p == '.') {
					p++;
					for (m = 0.1; &p && *p != '.' && *p != 'e'; p++) {
						floatValue = floatValue + (double)(*p - '0') * m;
						m *= 0.1;
					}
				}
				if (*p == 'e') {
					p++;
					if (*p == '-') {
						div = true;
						p++;
					}
					else if (*p == '+') {
						div = false;
						p++;
					}
					else {
						div = false;
					}
					for (pow = 0; *p; p++) {
						pow = pow * 10 + (int)(*p - '0');
					}
					for (m = 1.0, i = 0; i < pow; i++) {
						m *= 10.0;
					}
					if (div) {
						floatValue /= m;
					}
					else {
						floatValue *= m;
					}
				}
			}
			intValue = Math::ftol(floatValue);
		}
		else if (subtype & TOKENSUBTYPE_DECIMAL) {
			while (*p) {
				intValue = intValue * 10 + (*p - '0');
				p++;
			}
			floatValue = intValue;
		}
		else if (subtype & TOKENSUBTYPE_OCTAL) {
			p += 1;
			while (*p) {
				intValue = (intValue << 3) + (*p - '0');
				p++;
			}
			floatValue = intValue;
		}
		else if (subtype & TOKENSUBTYPE_HEX) {
			p += 2;
			while (*p) {
				intValue = intValue << 4;
				if (*p >= 'a' && *p <= 'f') {
					intValue += *p - 'a' + 10;
				}
				else if (*p >= 'A' && *p <= 'F') {
					intValue += *p - 'A' + 10;
				}
				else {
					intValue += *p - '0';
				}
				*p++;
			}
			floatValue = intValue;
		}
		else if (subtype & TOKENSUBTYPE_BINARY) {
			p += 2;
			while (*p) {
				intValue = (intValue << 1) + (*p - '0');
				p++;
			}
			floatValue = intValue;
		}
		subtype |= TOKENSUBTYPE_VALID;
	}
}
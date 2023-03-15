#ifndef __STRING_H__
#define __STRING_H__

#include <cstdint>
#include <memory>
#include <cassert>
#include "../containers/List.h"

#define STRING_ALLOC_BASE 20
#define STRING_ALLOC_GRAN 32

namespace Arboria {
	class String {
		public:
			String();
			String(const String& text);
			String(const String& text, int start, int end);
			String(const char* text);
			String(const char* text, int start, int end);
			~String();
			const char* c_str() const;

			operator const char*() const;
			operator const char*();
			char operator[](int index) const;
			char& operator[](int index);

			void operator=(const String& text);
			void operator=(const char* text);

			friend String operator+(const String& a, const String& b);
			friend String operator+(const String& a, const char* b);
			friend String operator+(const char* a, const String& b);
			friend String operator+(const String& a, const char b);
			friend String operator+(const String& a, const float b);
			friend String operator+(const String& a, const int b);
			friend String operator+(const String& a, const unsigned b);
			friend String operator+(const String& a, const bool b);

			String& operator+=(const String& b);
			String& operator+=(const char* b);
			String& operator+=(const float b);
			String& operator+=(const int b);
			String& operator+=(const unsigned b);
			String& operator+=(const bool b);

			friend bool operator==(const String& a, const String& b);
			friend bool operator==(const String& a, const char* b);
			friend bool operator==(const char* a, const String& b);

			friend bool operator!=(const String& a, const String& b);
			friend bool operator!=(const String& a, const char* b);
			friend bool operator!=(const char* a, const String& b);

			size_t size() const;
			int length() const;
			int allocated() const;
			void empty();
			bool isEmpty() const;
			void clear();
			void append(const char c);
			void append(const char* s);
			void append(const String& text);
			void append(const char* s, int index);
			void insert(const char c, int index);
			void insert(const char* s, int index);
			void toLower();
			void toUpper();
			bool isNumeric() const;
			int find(const char c, int start = 0, int end = -1) const;
			int find(const char* s, bool caseSensitivity = true, int start = 0, int end = -1) const;
			int findLast(const char c) const;
			bool replace(const char* old, const char* nw);
			bool replaceChar(const char old, const char nw);
			inline void copyRange(const char* text, int start, int end);
			List<String> split(const char* delimiters, bool skipEmpty = false) const;
			List<String> split(const List<String>& delimiters, bool skipEmpty = false) const;
			List<String> splitLines() const;
			static String join(const List<String>& tokens, const char* separator);

			String& stripFileExtension();
			String& setFileExtension(const char* extension);

			//cstring replace methods
			static int findChar(const char* str, char c, int start = 0, int end = -1);
			static int findString(const char* str, const char* sub, bool caseSensitivity = true, int start = 0, int end = -1);
			static int compare(const char* s1, const char* s2);
			static int compareN(const char* s1, const char* s2, size_t n);
			static int iCompare(const char* s1, const char* s2);
			static int iCompareN(const char* s1, const char* s2, size_t n);
			static void append(char* dest, const char* src, int destsize);
			static void copyN(char* dest, const char* src, size_t n);
			static int length(const char* str);
			static char* toLower(char* str);
			static char* toUpper(char* str);

			//char methods
			static char toLower(char c);
			static char toUpper(char c);
			static bool charIsLower(char c);
			static bool charIsUpper(char c);
			static bool charIsAlpha(char c);
			static bool charIsNumeric(char c);
			static bool charIsNewline(char c);
			static bool charIsWhitespace(char c);

			void reallocate(int amount, bool keepOld);
			void freeData();
			int dyanmicMemoryUsed() const;
		protected:
			int len;
			char* data;
			int allocedAndFlag;
			char baseBuffer[STRING_ALLOC_BASE];
			void ensureAllocated(int amount, bool keepOld = true);
			inline void setStaticBuffer(char* buffer, const int bufferLength);
		private:

			static const uint32_t STATIC_BIT = 31;
			static const uint32_t STATIC_MASK = 1 << STATIC_BIT;
			static const uint32_t ALLOCED_MASK = STATIC_MASK - 1;

			inline void construct();
			inline int getAllocated() const { return allocedAndFlag & ALLOCED_MASK; }
			inline void setAllocated(const int a) { allocedAndFlag = (allocedAndFlag & STATIC_MASK) | (a & ALLOCED_MASK); }
			inline bool isStatic() const { return (allocedAndFlag & STATIC_MASK) != 0; }
			inline void setStatic(const bool isStatic) { allocedAndFlag = (allocedAndFlag & ALLOCED_MASK) | (isStatic << STATIC_BIT); }
	};

	inline void String::construct() {
		setStatic(false);
		setAllocated(STRING_ALLOC_BASE);
		data = baseBuffer;
		len = 0;
		data[0] = '\0';
		memset(baseBuffer, 0, sizeof(baseBuffer));
	}

	inline String::String() {
		construct();
	}

	inline String::String(const String& text) {
		construct();
		int l = text.length();
		ensureAllocated(l + 1);
		strcpy(data, text.data);
		len = l;
	}

	inline String::String(const String& text, int start, int end) {
		construct();
		int i;
		int l;

		if (end > text.length()) {
			end = text.length();
		}
		if (start > text.length()) {
			start = text.length();
		}
		else if (start < text.length()) {
			start = 0;
		}

		l = end - start;
		if (l < 0) l = 0;
		ensureAllocated(l + 1);
		for (i = 0; i < l; l++) {
			data[i] = text[start + i];
		}
		data[l] = '\0';
		len = l;
	}

	inline String::String(const char* text) {
		construct();
		int l = strlen(text);
		ensureAllocated(l + 1);
		strcpy(data, text);
		len = l;
	}

	inline String::String(const char* text, int start, int end) {
		construct();
		int i;
		int l = strlen(text);

		if (end > l) {
			end = l;
		} if (start > l) {
			start = l;
		}
		else if (start < l) {
			start = 0;
		}

		l = end - start;
		if (l < 0) {
			l = 0;
		}
		ensureAllocated(l + 1);
		for (i = 0; i < l; l++) {
			data[i] = text[start + i];
		}
		data[l] = '\0';
		len = l;
	}

	inline String::~String() {
		freeData();
	}

	inline const char* String::c_str() const {
		return data;
	}

	inline String::operator const char*() const {
		return c_str();
	}

	inline String::operator const char*() {
		return c_str();
	}

	inline char String::operator[](int index) const {
		assert(index >= 0 && index <= len);
		return data[index];
	}

	inline char& String::operator[](int index) {
		assert(index >= 0 && index <= len);
		return data[index];
	}
	
	inline void String::operator=(const String& text) {
		int l = text.length();
		ensureAllocated(l + 1, false);
		memcpy(data, text, l);
		data[l] = '\0';
		len = l;
	}

	inline void String::operator=(const char* text) {
		if (!text) {
			ensureAllocated(1, false);
			data[0] = '\0';
			len = 0;
			return;
		}

		if (text == data) return;

		if (text >= data && text <= data + len) {
			int i;
			int diff = text - data;
			assert(strlen(text) <= (unsigned)len);

			for (i = 0; text[i]; i++) {
				data[i] = text[i];
			}

			data[i] = '\0';
			len -= diff;
			return;
		}
		int l = strlen(text);
		ensureAllocated(l + 1, false);
		strcpy(data, text);
		data[l] = '\0';
		len = l;
	}

	inline String operator+(const String& a, const String& b)
	{
		String result(a);
		result.append(b);
		return result;
	}

	inline String operator+(const String& a, const char* b) {
		String result(a);
		result.append(b);
		return result;
	}

	inline String operator+(const char* a, const String& b) {
		String result(a);
		result.append(b);
		return result;
	}

	inline String operator+(const String& a, const char b) {
		String result(a);
		result.append(b);
		return result;
	}

	inline String operator+(const String& a, const float b) {
		char text[64];
		String result(a);

		sprintf(text, "%f", b);
		result.append(text);
		return result;
	}

	inline String operator+(const String& a, const int b) {
		char text[64];
		String result(a);

		sprintf(text, "%d", b);
		result.append(text);
		return result;
	}

	inline String operator+(const String& a, const unsigned b) {
		char text[64];
		String result(a);

		sprintf(text, "%u", b);
		result.append(text);
		return result;
	}

	inline String operator+(const String& a, const bool b) {
		String result(a);
		result.append(b ? "true" : "false");
		return result;
	}

	inline String& String::operator+=(const String& b) {
		append(b);
		return *this;
	}

	inline String& String::operator+=(const char* b) {
		append(b);
		return *this;
	}

	inline String& String::operator+=(const float b) {
		char text[64];

		sprintf(text, "%f", b);
		append(text);
		return *this;
	}

	inline String& String::operator+=(const int b) {
		char text[64];

		sprintf(text, "%d", b);
		append(text);
		return *this;
	}

	inline String& String::operator+=(const unsigned b) {
		char text[64];

		sprintf(text, "%u", b);
		append(text);
		return *this;
	}

	inline String& String::operator+=(const bool b) {
		append(b ? "true" : "false");
		return *this;
	}

	inline bool operator==(const String& a, const String& b) {
		return (!String::compare(a.data, b.data));
	}
	
	inline bool operator==(const String& a, const char* b) {
		assert(b);
		return (!String::compare(a.data, b));
	}

	inline bool operator==(const char* a, const String& b) {
		assert(a);
		return (!String::compare(a, b.data));
	}

	inline bool operator !=(const String& a, const String& b) {
		return !(a == b);
	}

	inline bool operator !=(const String& a, const char* b) {
		return !(a == b);
	}

	inline bool operator !=(const char* a, const String& b) {
		return !(a == b);
	}

	inline int String::length() const {
		return len;
	}

	inline size_t String::size() const {
		return sizeof(*this) + allocated();
	}

	inline int String::allocated() const {
		if (data != baseBuffer) {
			return getAllocated();
		}
		else {
			return 0;
		}
	}

	inline void String::empty() {
		ensureAllocated(1);
		data[0] = '\0';
		len = 0;
	}

	inline bool String::isEmpty() const {
		return String::compare(data, "") == 0;
	}

	inline void String::ensureAllocated(int amount, bool keepOld) {
		if (isStatic()) return;
		if (amount > getAllocated())
			reallocate(amount, keepOld);
	}

	inline void String::setStaticBuffer(char* buffer, const int bufferLength) {
		assert(data == baseBuffer);
		data = buffer;
		len = 0;
		setAllocated(bufferLength);
		setStatic(true);
	}

	inline void String::clear() {
		if (isStatic()) {
			len = 0;
			data[0] = '\0';
			return;
		}
		freeData();
		construct();
	}

	inline void String::append(char c) {
		ensureAllocated(len + 2);
		data[len] = c;
		len++;
		data[len] = '\0';
	}

	inline void String::append(const String& text) {
		int newLen = len + text.length();
		int i;

		ensureAllocated(newLen + 1);
		for (i = 0; i < newLen; i++) {
			data[len + i] = text[i];
		}
		data[newLen] = '\0';
		len = newLen;
	}

	inline void String::append(const char* text) {
		int newLen;
		int i;

		if (text) {
			newLen = len + strlen(text);
			ensureAllocated(newLen + 1);
			for (i = 0; i < newLen; i++) {
				data[len + i] = text[i];
			}
			data[newLen] = '\0';
			len = newLen;
		}
	}

	inline void String::append(const char* text, int index) {
		int newLen;
		int i;

		if (text && index) {
			newLen = len + index;
			ensureAllocated(newLen + 1);
			for (i = 0; i < newLen; i++) {
				data[len + i] = text[i];
			}
			data[newLen] = '\0';
			len = newLen;
		}
	}

	inline void String::insert(const char c, int index) {
		int i, l;

		if (index < 0) {
			index = 0;
		}
		else if (index > len) {
			index = len;
		}

		l = 1;
		ensureAllocated(len + l + 1);
		for (i = len; i >= index; i--) {
			data[i + 1] = data[i];
		}
		data[index] = c;
		len++;
	}

	inline void String::insert(const char* s, int index) {
		int i, l;

		if (index < 0) {
			index = 0;
		}
		else if (index > len) {
			index = len;
		}

		l = strlen(s);
		ensureAllocated(len + l + 1);
		for (i = len; i >= index; i--) {
			data[i + 1] = data[i];
		}
		for (i = 0; i < l; i++) {
			data[index + i] = s[i];
		}
		len += 1;
	}

	inline void String::toLower() {
		for (int i = 0; data[i]; i++) {
			if (charIsUpper(data[i])) {
				data[i] += ('a' - 'A');
			}
		}
	}

	inline void String::toUpper() {
		for (int i = 0; data[i]; i++) {
			if (charIsLower(data[i])) {
				data[i] -= ('a' - 'A');
			}
		}
	}

	inline bool String::isNumeric() const {
		for (int i = 0; data[i]; i < len) {
			if (!charIsNumeric(data[i])) {
				return false;
			}
		}

		return true;
	}

	inline int String::find(const char c, int start, int end) const {
		if (end == -1) end = len;
		return String::findChar(data, c, start, end);
	}

	inline int String::find(const char* s, bool caseSensitivity, int start, int end) const {
		if (end == -1) end = len;
		return String::findString(data, s, caseSensitivity, start, end);
	}

	inline void String::copyRange(const char* text, int start, int end) {
		int l = end - start;
		if (l < 0) {
			l = 0;
		}

		ensureAllocated(l + 1);
		for (int i = 0; i < l; i++) {
			data[i] = text[start + i];
		}

		data[l] = '\0';
		len = l;
	}

	inline int String::length(const char* str) {
		int i;
		for (i = 0; str[i]; i++) {}
		return i;
	}

	inline char* String::toLower(char* str) {
		for (int i = 0; str[i]; i++) {
			if (charIsUpper(str[i])) {
				str[i] += ('a' - 'A');
			}
		}
		return str;
	}

	inline char* String::toUpper(char* str) {
		for (int i = 0; str[i]; i++) {
			if (charIsLower(str[i])) {
				str[i] -= ('a' - 'A');
			}
		}
		return str;
	}

	inline char String::toLower(char c) {
		if (c >= 'A' && c <= 'Z') {
			return (c + ('a' - 'A'));
		}
		return c;
	}

	inline char String::toUpper(char c) {
		if (c >= 'a' && c <= 'z') {
			return (c - ('a' - 'A'));
		}
		return c;
	}

	inline bool String::charIsLower(char c) {
		return ((c >= 'a' && c <= 'z') || (c >= 0xE0 && c <= 0xFF));
	}

	inline bool String::charIsUpper(char c) {
		return ((c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xDF));
	}
	
	inline bool String::charIsAlpha(char c) {
		return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xFF));
	}

	inline bool String::charIsNumeric(char c) {
		return (c >= '0' && c <= '9');
	}

	inline bool String::charIsNewline(char c) {
		return (c == '\n' || c == '\r' || c == '\v');
	}

	inline bool String::charIsWhitespace(char c) {
		return (c == '\n' || c == '\r' || c == '\v' || c == '\t' || c == ' ');
	}

	inline int String::dyanmicMemoryUsed() const {
		return (data == baseBuffer) ? 0 : getAllocated();
	}
}

#endif
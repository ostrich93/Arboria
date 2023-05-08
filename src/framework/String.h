#ifndef STRING_H
#define STRING_H

#include <cstdint>
#include <memory>
#include <cassert>
#include "../containers/List.h"

const int STRING_ALLOC_BASE = 32 - 8 - sizeof(char*);
const int STRING_ALLOC_GRAN = 32;

namespace Arboria {
	class StringData {
		public:
			StringData() : len(0), referenceCount(0), data(NULL), alloced(0) {}
			~StringData() {
				if (data) {
					delete[] data;
				}
			}
			void addReference() { referenceCount++; }
			bool deleteReference() {
				referenceCount--;
				if (referenceCount < 0) {
					delete this;
					return true;
				}
				return false;
			}
			int len;
			int referenceCount;
			char* data;
			int alloced;
	};

	class String {
		public:
			String();
			String(const String& text);
			String(const String& text, int start, int end);
			String(const char* text);
			String(const char* text, int start, int end);
			explicit String(const bool b);
			explicit String(const char c);
			explicit String(const int i);
			explicit String(const unsigned u);
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
			bool isEmpty() const;
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
			int compare(const char* text) const;
			int compareN(const char* text, size_t n) const;
			int iCompare(const char* text) const;
			int iCompareN(const char* text, size_t n);
			bool replace(const char* old, const char* nw);
			bool replaceChar(const char old, const char nw);
			void copyRange(const char* text, int start, int end);
			//List<String> split(const char* delimiters, bool skipEmpty = false) const;
			//List<String> split(const List<String>& delimiters, bool skipEmpty = false) const;
			//List<String> splitLines() const;
			static String join(const List<String>& tokens, const char* separator);

			bool stripLeading(const char c);
			String& stripFileExtension();
			String& setFileExtension(const char* extension);

			static int getHash(const char* string);
			static int getHash(const char* string, int length);

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
			int dynamicMemoryUsed() const;
		protected:
			StringData* m_data;
			void ensureAllocated(int amount, bool keepOld = true);
			void ensureDataWritable();
	};

	inline void String::ensureAllocated(int amount, bool keepOld) {
		if (!m_data) 
			m_data = new StringData();

		ensureDataWritable();
		if (amount < m_data->alloced)
			return;

		reallocate(amount, keepOld);
	}

	inline void String::ensureDataWritable() {
		assert(m_data);
		StringData* oldData;
		int len;

		if (!m_data->referenceCount) return;

		oldData = m_data;
		len = length();

		m_data = new StringData;
		ensureAllocated(len + 1, false);
		strncpy(m_data->data, oldData->data, static_cast<size_t>(len) + 1);
		m_data->len = len;
		oldData->deleteReference();
	}

	inline String::String() : m_data(NULL) {
		ensureAllocated(1);
		m_data->data[0] = '\0';
	}

	inline String::String(const String& text) : m_data(NULL) {
		m_data = text.m_data;
		m_data->addReference();
	}

	inline String::String(const String& text, int start, int end) : m_data(NULL){
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
		if (l < 0)
			l = 0;
		ensureAllocated(l + 1);
		for (i = 0; i < l; i++) {
			m_data->data[i] = text[start + i];
		}
		m_data->data[l] = '\0';
		m_data->len = l;
	}

	inline String::String(const char* text) : m_data(NULL) {
		int l = static_cast<int>(strlen(text));
		ensureAllocated(l + 1);
		strcpy(m_data->data, text);
		m_data->len = l;
	}

	inline String::String(const char* text, int start, int end) : m_data(NULL) {
		int i;
		int l = static_cast<int>(strlen(text));

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
		for (i = 0; i < l; i++) {
			m_data->data[i] = text[start + i];
		}
		m_data->data[l] = '\0';
		m_data->len = l;
	}

	inline String::String(const bool b) : m_data(NULL){
		ensureAllocated(2);
		m_data->data[0] = b ? '1' : '0';
		m_data->data[1] = '\0';
		m_data->len = 1;
	}

	inline String::String(const char c) : m_data(NULL) {
		ensureAllocated(2);
		m_data->data[0] = c;
		m_data->data[1] = '\0';
		m_data->len = 1;
	}

	inline String::String(const int i) : m_data(NULL) {
		char text[64];
		int l;

		sprintf(text, "%d", i);
		l = static_cast<int>(strlen(text));
		ensureAllocated(l + 1);
		strcpy(m_data->data, text);
		m_data->len = l;
	}

	inline String::String(const unsigned u) : m_data(NULL) {
		char text[64];
		int l;

		sprintf(text, "%u", u);
		l = static_cast<int>(strlen(text));
		ensureAllocated(l + 1);
		strcpy(m_data->data, text);
		m_data->len = l;
	}

	inline String::~String() {
		if (m_data) {
			m_data->deleteReference();
			m_data = NULL;
		}
		//freeData();
	}

	inline size_t String::size() const {
		return sizeof(*this) + allocated();
	}

	inline int String::allocated() const {
		if (m_data != NULL) return m_data->alloced;
		return 0;
	}

	inline const char* String::c_str() const {
		assert(m_data);
		return m_data->data;
	}

	inline String::operator const char* () const {
		return c_str();
	}

	inline String::operator const char* () {
		return c_str();
	}

	inline char String::operator[](int index) const {
		assert(index >= 0 && index <= m_data->len);
		return m_data->data[index];
	}

	inline char& String::operator[](int index) {
		assert(index >= 0 && index <= m_data->len);
		return m_data->data[index];
	}

	inline void String::operator=(const String& text) {
		text.m_data->addReference();
		m_data->deleteReference();
		m_data = text.m_data;
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
		return (!String::compare(a.c_str(), b.c_str()));
	}

	inline bool operator==(const String& a, const char* b) {
		assert(b);
		return (!String::compare(a.c_str(), b));
	}

	inline bool operator==(const char* a, const String& b) {
		assert(a);
		return (!String::compare(a, b.c_str()));
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
		if (m_data != NULL) return m_data->len;
		return 0;
	}

	inline bool String::isEmpty() const {
		return String::compare(c_str(), "") == 0;
	}

	inline void String::append(char c) {
		ensureAllocated(m_data->len + 2);
		m_data->data[m_data->len] = c;
		m_data->len++;
		m_data->data[m_data->len] = '\0';
	}

	inline void String::append(const String& text) {
		int newLen = length() + text.length();
		int i;

		ensureAllocated(newLen + 1);
		strcat(m_data->data, text.c_str());
		m_data->len = newLen;
	}

	inline void String::append(const char* text) {
		assert(text);
		int newLen;

		if (text) {
			newLen = length() +static_cast<int>(strlen(text));
			ensureAllocated(newLen + 1);
			strcat(m_data->data, text);
			m_data->len = newLen;
		}
	}

	inline void String::append(const char* text, int index) {
		int newLen;
		int i;

		if (text && index) {
			newLen = length() + index;
			ensureAllocated(newLen + 1);
			for (i = 0; text[i] && i < index; i++) {
				m_data->data[m_data->len + i] = text[i];
			}
			m_data->data[newLen] = '\0';
			m_data->len = newLen;
		}
	}

	inline void String::insert(const char c, int index) {
		int i, l;
		int _length = length();

		if (index < 0) {
			index = 0;
		}
		else if (index > _length) {
			index = _length;
		}

		l = 1;
		ensureAllocated(_length + l + 1);
		for (i = _length; i >= index; i--) {
			m_data->data[i + 1] = m_data->data[i];
		}
		m_data->data[index] = c;
		m_data->len++;
	}

	inline void String::insert(const char* s, int index) {
		int i, l;
		int _length = length();

		if (index < 0) {
			index = 0;
		}
		else if (index > _length) {
			index = _length;
		}

		l = static_cast<int>(strlen(s));
		ensureAllocated(_length + l + 1);
		for (i = _length; i >= index; i--) {
			m_data->data[i + 1] = m_data->data[i];
		}
		for (i = 0; i < l; i++) {
			m_data->data[index + i] = s[i];
		}
		m_data->len += 1;
	}

	inline void String::toLower() {
		for (int i = 0; m_data->data[i]; i++) {
			if (charIsUpper(m_data->data[i])) {
				m_data->data[i] += ('a' - 'A');
			}
		}
	}

	inline void String::toUpper() {
		for (int i = 0; m_data->data[i]; i++) {
			if (charIsLower(m_data->data[i])) {
				m_data->data[i] -= ('a' - 'A');
			}
		}
	}

	inline bool String::isNumeric() const {
		for (int i = 0; i < length(); i++) {
			if (!charIsNumeric(m_data->data[i])) {
				return false;
			}
		}

		return true;
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

	inline int String::getHash(const char* string) {
		int i, hash = 0;
		for (i = 0; *string != '\0'; i++) {
			hash += (*string++) * (i + 119);
		}
		return hash;
	}

	inline int String::getHash(const char* string, int length) {
		int i, hash = 0;
		for (i = 0; i < length; i++) {
			hash += (*string++) * (i + 119);
		}
		return hash;
	}

	inline void String::copyRange(const char* text, int start, int end) {
		int l = end - start;
		if (l < 0) {
			l = 0;
		}

		ensureAllocated(l + 1);
		for (int i = 0; i < l; i++) {
			m_data->data[i] = text[start + i];
		}

		m_data->data[l] = '\0';
		m_data->len = l;
	}

	inline int String::dynamicMemoryUsed() const {
		return (m_data != NULL) ? m_data->alloced : 0;
	}
}

#endif
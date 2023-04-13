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
			void freeData();
			int dyanmicMemoryUsed() const;
		protected:
			int len;
			char* data;
			int allocedAndFlag;
			char baseBuffer[STRING_ALLOC_BASE];
			void ensureAllocated(int amount, bool keepOld = true);
			void setStaticBuffer(char* buffer, const int bufferLength);
		private:

			static const uint32_t STATIC_BIT = 31;
			static const uint32_t STATIC_MASK = 1 << STATIC_BIT;
			static const uint32_t ALLOCED_MASK = STATIC_MASK - 1;

			void construct();
			int getAllocated() const;
			void setAllocated(const int a);
			bool isStatic() const;
			void setStatic(const bool isStatic);
	};

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
}

#endif
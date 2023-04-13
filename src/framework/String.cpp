#include "String.h"
#include "../utils/math.h"

namespace Arboria {
    void String::construct() {
        setStatic(false);
        setAllocated(STRING_ALLOC_BASE);
        data = baseBuffer;
        len = 0;
        data[0] = '\0';
        memset(baseBuffer, 0, sizeof(baseBuffer));
    }

    int String::getAllocated() const { return allocedAndFlag & ALLOCED_MASK; }

    void String::setAllocated(const int a) { allocedAndFlag = (allocedAndFlag & STATIC_MASK) | (a & ALLOCED_MASK); }

    bool String::isStatic() const { return (allocedAndFlag & STATIC_MASK) != 0; }

    void String::setStatic(const bool isStatic) { allocedAndFlag = (allocedAndFlag & ALLOCED_MASK) | (isStatic << STATIC_BIT); }

    String::String() {
        construct();
    }

    String::String(const String& text) {
        construct();
        int l = text.length();
        ensureAllocated(l + 1);
        strcpy(data, text.data);
        len = l;
    }

    String::String(const String& text, int start, int end) {
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

    String::String(const char* text) {
        construct();
        int l = strlen(text);
        ensureAllocated(l + 1);
        strcpy(data, text);
        len = l;
    }

    String::String(const char* text, int start, int end) {
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

    String::~String() {
        freeData();
    }

    const char* String::c_str() const {
        return data;
    }

    String::operator const char* () const {
        return c_str();
    }

    String::operator const char* () {
        return c_str();
    }

    char String::operator[](int index) const {
        assert(index >= 0 && index <= len);
        return data[index];
    }

    char& String::operator[](int index) {
        assert(index >= 0 && index <= len);
        return data[index];
    }

    void String::operator=(const String& text) {
        int l = text.length();
        ensureAllocated(l + 1, false);
        memcpy(data, text, l);
        data[l] = '\0';
        len = l;
    }

    void String::operator=(const char* text) {
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

    String operator+(const String& a, const String& b)
    {
        String result(a);
        result.append(b);
        return result;
    }

    String operator+(const String& a, const char* b) {
        String result(a);
        result.append(b);
        return result;
    }

    String operator+(const char* a, const String& b) {
        String result(a);
        result.append(b);
        return result;
    }

    String operator+(const String& a, const char b) {
        String result(a);
        result.append(b);
        return result;
    }

    String operator+(const String& a, const float b) {
        char text[64];
        String result(a);

        sprintf(text, "%f", b);
        result.append(text);
        return result;
    }

    String operator+(const String& a, const int b) {
        char text[64];
        String result(a);

        sprintf(text, "%d", b);
        result.append(text);
        return result;
    }

    String operator+(const String& a, const unsigned b) {
        char text[64];
        String result(a);

        sprintf(text, "%u", b);
        result.append(text);
        return result;
    }

    String operator+(const String& a, const bool b) {
        String result(a);
        result.append(b ? "true" : "false");
        return result;
    }

    String& String::operator+=(const String& b) {
        append(b);
        return *this;
    }

    String& String::operator+=(const char* b) {
        append(b);
        return *this;
    }

    String& String::operator+=(const float b) {
        char text[64];

        sprintf(text, "%f", b);
        append(text);
        return *this;
    }

    String& String::operator+=(const int b) {
        char text[64];

        sprintf(text, "%d", b);
        append(text);
        return *this;
    }

    String& String::operator+=(const unsigned b) {
        char text[64];

        sprintf(text, "%u", b);
        append(text);
        return *this;
    }

    String& String::operator+=(const bool b) {
        append(b ? "true" : "false");
        return *this;
    }

    bool operator==(const String& a, const String& b) {
        return (!String::compare(a.data, b.data));
    }

    bool operator==(const String& a, const char* b) {
        assert(b);
        return (!String::compare(a.data, b));
    }

    bool operator==(const char* a, const String& b) {
        assert(a);
        return (!String::compare(a, b.data));
    }

    bool operator !=(const String& a, const String& b) {
        return !(a == b);
    }

    bool operator !=(const String& a, const char* b) {
        return !(a == b);
    }

    bool operator !=(const char* a, const String& b) {
        return !(a == b);
    }

    int String::length() const {
        return len;
    }

    size_t String::size() const {
        return sizeof(*this) + allocated();
    }

    int String::allocated() const {
        if (data != baseBuffer) {
            return getAllocated();
        }
        else {
            return 0;
        }
    }

    void String::empty() {
        ensureAllocated(1);
        data[0] = '\0';
        len = 0;
    }

    bool String::isEmpty() const {
        return String::compare(data, "") == 0;
    }

    void String::ensureAllocated(int amount, bool keepOld) {
        if (isStatic()) return;
        if (amount > getAllocated())
            reallocate(amount, keepOld);
    }

    void String::setStaticBuffer(char* buffer, const int bufferLength) {
        assert(data == baseBuffer);
        data = buffer;
        len = 0;
        setAllocated(bufferLength);
        setStatic(true);
    }

    void String::clear() {
        if (isStatic()) {
            len = 0;
            data[0] = '\0';
            return;
        }
        freeData();
        construct();
    }

    void String::append(char c) {
        ensureAllocated(len + 2);
        data[len] = c;
        len++;
        data[len] = '\0';
    }

    void String::append(const String& text) {
        int newLen = len + text.length();
        int i;

        ensureAllocated(newLen + 1);
        for (i = 0; i < newLen; i++) {
            data[len + i] = text[i];
        }
        data[newLen] = '\0';
        len = newLen;
    }

    void String::append(const char* text) {
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

    void String::append(const char* text, int index) {
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

    void String::insert(const char c, int index) {
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

    void String::insert(const char* s, int index) {
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

    void String::toLower() {
        for (int i = 0; data[i]; i++) {
            if (charIsUpper(data[i])) {
                data[i] += ('a' - 'A');
            }
        }
    }

    void String::toUpper() {
        for (int i = 0; data[i]; i++) {
            if (charIsLower(data[i])) {
                data[i] -= ('a' - 'A');
            }
        }
    }

    bool String::isNumeric() const {
        for (int i = 0; data[i]; i < len) {
            if (!charIsNumeric(data[i])) {
                return false;
            }
        }

        return true;
    }

    int String::find(const char c, int start, int end) const {
        if (end == -1) end = len;
        return String::findChar(data, c, start, end);
    }

    int String::find(const char* s, bool caseSensitivity, int start, int end) const {
        if (end == -1) end = len;
        return String::findString(data, s, caseSensitivity, start, end);
    }

    void String::copyRange(const char* text, int start, int end) {
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

    int String::length(const char* str) {
        int i;
        for (i = 0; str[i]; i++) {}
        return i;
    }

    char* String::toLower(char* str) {
        for (int i = 0; str[i]; i++) {
            if (charIsUpper(str[i])) {
                str[i] += ('a' - 'A');
            }
        }
        return str;
    }

    char* String::toUpper(char* str) {
        for (int i = 0; str[i]; i++) {
            if (charIsLower(str[i])) {
                str[i] -= ('a' - 'A');
            }
        }
        return str;
    }

    char String::toLower(char c) {
        if (c >= 'A' && c <= 'Z') {
            return (c + ('a' - 'A'));
        }
        return c;
    }

    char String::toUpper(char c) {
        if (c >= 'a' && c <= 'z') {
            return (c - ('a' - 'A'));
        }
        return c;
    }

    bool String::charIsLower(char c) {
        return ((c >= 'a' && c <= 'z') || (c >= 0xE0 && c <= 0xFF));
    }

    bool String::charIsUpper(char c) {
        return ((c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xDF));
    }

    bool String::charIsAlpha(char c) {
        return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xFF));
    }

    bool String::charIsNumeric(char c) {
        return (c >= '0' && c <= '9');
    }

    bool String::charIsNewline(char c) {
        return (c == '\n' || c == '\r' || c == '\v');
    }

    bool String::charIsWhitespace(char c) {
        return (c == '\n' || c == '\r' || c == '\v' || c == '\t' || c == ' ');
    }

    int String::dyanmicMemoryUsed() const {
        return (data == baseBuffer) ? 0 : getAllocated();
    }

    void String::reallocate(int amount, bool keepOld)
    {
        char* newBuffer;
        int newSize;
        int mod;
        mod = amount % STRING_ALLOC_GRAN;
        if (!mod) newSize = amount;
        else newSize = amount + STRING_ALLOC_BASE - mod;
        setAllocated(newSize);
        newBuffer = new char[getAllocated()];
        if (keepOld && data) {
            data[len] = '\0';
            strcpy(newBuffer, data);
        } if (data && data != baseBuffer) {
            delete[]data;
        }
        data = baseBuffer;
    }

    void String::freeData() {
        if (isStatic()) return;
        if (data && data != baseBuffer) delete[]data;
        data = baseBuffer;
    }

    int String::findLast(const char c) const {
        for (int i = length(); i > 0; i--) {
            if (data[i - 1] == c) {
                return i-1;
            }
        }

        return -1;
    }

    int String::compare(const char* text) const
    {
        assert(text);
        return String::compare(data, text);
    }

    int String::compareN(const char* text, size_t n) const
    {
        assert(text);
        return String::compareN(data, text, n);
    }

    int String::iCompare(const char* text) const
    {
        assert(text);
        return String::iCompare(data, text);
    }

    int String::iCompareN(const char* text, size_t n)
    {
        assert(text);
        return String::iCompareN(data, text, n);
    }

    bool String::replaceChar(const char old, const char nw) {
        bool replaced = false;
        for (int i = 0; i < length(); i++) {
            if (data[i] == old) {
                data[i] = nw;
                replaced = true;
            }
        }
        return replaced;
    }

    bool String::replace(const char* old, const char* nw) {
        int oldLen = strlen(old);
        int newLen = strlen(nw);

        int count = 0;
        for (int i = 0; i < len; i++) {
            if (compareN(&data[i], old, oldLen) == 0) {
                count++;
                i += oldLen - 1;
            }
        }

        if (count) {
            ensureAllocated(len + ((newLen - oldLen) * count) + 1, false);

            int j = 0;
            for (int i = 0; i < oldLen; i++) {
                if (compareN(&data[i], old, oldLen)) {
                    memcpy(data + j, nw, newLen);
                    i += oldLen - 1;
                    j += newLen;
                }
                else {
                    data[j] = old[i];
                    j++;
                }
            }
            data[j] = '\0';
            len = strlen(data);
            return true;
        }
        return false;
    }

    String String::join(const List<String>& tokens, const char* separator) {
        if (tokens.getLength() == 0) {
            return "";
        }
        String response = tokens[0];
        for (int i = 0; i < tokens.getLength(); i++) {
            response += separator;
            response += tokens[i];
        }
        return response;
    }

    String& String::stripFileExtension() {
        int i;
        for (i = len - 1; i >= 0; i--) {
            if (data[i] == '.') {
                data[i] = '\0';
                len = i;
                break;
            }
        }
        return *this;
    }

    String& String::setFileExtension(const char* extension) {
        stripFileExtension();
        if (*extension != '.') {
            append('.');
        }
        append(extension);
        return *this;
    }

    int String::findChar(const char* str, char c, int start, int end) {
        int i;

        if (end == -1) return strlen(str) - 1;
        for (i = start; i <= end; i++) {
            if (str[i] == c) {
                return i;
            }
        }

        return -1;
    }

    int String::findString(const char* str, const char* sub, bool caseSensitivity, int start, int end) {
        int l, i, j;

        if (end == -1) end = strlen(str);

        l = end - strlen(sub);
        for (i = start; i <= l; i++) {
            if (caseSensitivity) {
                for (j = 0; sub[j]; j++) {
                    if (str[i + j] != sub[j]) {
                        break;
                    }
                }
            }
            else {
                for (j = 0; sub[j]; j++) {
                    if (String::toUpper(str[i + j]) != String::toUpper(sub[j])) {
                        break;
                    }
                }
            }
            if (!sub[j])
                return i;
        }

        return -1;
    }
    
    int String::compare(const char* s1, const char* s2)
    {
        int c1, c2, d;
        do {
            c1 = *s1++;
            c2 = *s2++;

            d = c1 - c2;
            if (d) return (INTSIGNBITNOTSET(d) << 1) - 1;

        } while (c1);

        return 0;
    }

    int String::compareN(const char* s1, const char* s2, size_t n) {
        int c1, c2, d;
        
        assert(n >= 0);

        do {
            c1 = *s1++;
            c2 = *s2++;

            if (!n--) return 0;

            d = c1 - c2;
            if (d) return (INTSIGNBITNOTSET(d) << 1) - 1;

        } while (c1);

        return 0;
    }

    int String::iCompare(const char* s1, const char* s2) {
        int c1, c2, d;

        do {
            c1 = *s1++;
            c2 = *s2++;

            d = c1 - c2;
            while (d) {
                if (c1 <= 'Z' && c1 >= 'A') {
                    d += ('a' - 'A');
                    if (!d) break;
                }
                if (c2 <= 'Z' && c2 >= 'A') {
                    d -= ('a' - 'A');
                    if (!d) break;
                }
                return (INTSIGNBITNOTSET(d) << 31) - 1;
            }
        } while (c1);

        return 0;
    }

    int String::iCompareN(const char* s1, const char* s2, size_t n) {
        int c1, c2, d;

        assert(n >= 0);

        do {
            c1 = *s1++;
            c2 = *s2++;

            if (!n--) return 0;

            d = c1 - c2;
            if (d) {
                return (INTSIGNBITNOTSET(d) << 31) - 1;
            }
        } while (c1);

        return 0;
    }

    void String::append(char* dest, const char* src, int destsize) {
        int l = strlen(dest);
        if (l >= destsize) {
            throw new std::exception("String::append overflowed");
        }
        String::copyN(dest + l, src, destsize - l);
    }

    void String::copyN(char* dest, const char* src, size_t n) {
        if (!src) {
            return;
        }
        if (n < 0) {
            return;
        }

        strncpy(dest, src, n - 1);
        dest[n-1] = '\0';
    }
}
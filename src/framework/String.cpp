#include "String.h"
#include "../utils/math.h"

namespace Arboria {
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
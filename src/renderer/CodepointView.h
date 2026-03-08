#ifndef CODEPOINT_VIEW_H
#define CODEPOINT_VIEW_H

#include "../framework/String.h"
#include "../utils/math.h"

namespace Arboria {

	int utf8GetCodepointSize(String& v) {
		if (v.size() >= 1 && !(v[0] & 0x80))
			return 1;
		if (v.size() >= 2 && ((v[0] & 0xE0) == 0xC0))
			return 2;
		if (v.size() >= 3 && ((v[0] & 0xF0) == 0xE0))
			return 3;
		if (v.size() >= 4 && ((v[0] & 0xF8) == 0xF0))
			return 4;
		return 0;
	}

	String& utf8Truncate(String& v, size_t size) {
		auto trunc = v.substring(0, size);
		for (size_t i = 0; i < trunc.length();) {
			auto length = utf8GetCodepointSize(v);
			if (!length)
				return trunc.substring(0, i);

			i += length;
		}

		return trunc;
	}

	class CodepointView {
	private:
		String& _str;
	public:
		class iterator {
		private:
			String& _str;
			size_t _index;

		public:
			iterator(String str, size_t index) : _str(str), _index(index) {}

			bool operator==(const iterator& rhs) const {
				return _index == rhs._index;
			}
			bool operator!=(const iterator& rhs) const {
				return _index != rhs._index;
			}
			char32_t operator*() const {
				return getNextCodepoint(&_str[_index], nullptr);
			}
			iterator& operator++() {
				if (_index < _str.length()) {
					const char* nextchar;
					getNextCodepoint(&_str[_index], &nextchar);
					_index = Math::iMin(nextchar - _str, _str.length());
				}
				return *this;
			}
			iterator operator++(int) {
				auto result = *this;
				if (_index < _str.size()) {
					const char* nextchar;
					getNextCodepoint(&_str[_index], &nextchar);
					_index = Math::iMin(nextchar - _str, _str.length());
				}
				return result;
			}

			size_t getIndex() const {
				return _index;
			}

			static char32_t getNextCodepoint(const char* char_ptr, const char** next_ptr) {
				int32_t result;
				int32_t numBytes;

				if (!(char_ptr[0] & 0x80)) {
					numBytes = 1;
					result = char_ptr[0];
				}
				else if ((char_ptr[0] & 0xE0) == 0xC0) {
					numBytes = 2;
					result = ((char_ptr[0] & 0x1f) << 6) | (char_ptr[1] & 0x3f);
				}
				else if ((char_ptr[0] & 0xF0) == 0xE0) {
					numBytes = 3;
					result = ((char_ptr[0] & 0xF) << 12) | ((char_ptr[1] & 0x3f) << 6) | (char_ptr[2] & 0x3f);
				}
				else if ((char_ptr[0] & 0xF8) == 0xF0) {
					numBytes = 4;
					result = ((char_ptr[0] & 0x07) << 18) | ((char_ptr[1] & 0x3f) << 12) | ((char_ptr[2] & 0x3f) << 6) | (char_ptr[3] & 0x3f);
				}
				else {
					numBytes = 1;
					result = ' ';
				}

				if (next_ptr != nullptr) {
					*next_ptr = char_ptr + numBytes;
				}
				return result;
			}
		};
		CodepointView(String str) : _str(utf8Truncate(str, str.size())) {

		}
		iterator begin() const {
			return iterator(_str, 0);
		}
		iterator end() const {
			return iterator(_str, _str.size());
		}
	};
}

#endif
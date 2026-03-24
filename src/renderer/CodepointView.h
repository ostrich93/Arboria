#ifndef CODEPOINT_VIEW_H
#define CODEPOINT_VIEW_H

#include "../framework/String.h"
#include "../utils/math.h"
#include <string.h>

namespace Arboria {

	int utf8GetCodepointSize(std::string_view v) {
		if (v.length() >= 1 && !(v[0] & 0x80))
			return 1;
		if (v.length() >= 2 && ((v[0] & 0xE0) == 0xC0))
			return 2;
		if (v.length() >= 3 && ((v[0] & 0xF0) == 0xE0))
			return 3;
		if (v.length() >= 4 && ((v[0] & 0xF8) == 0xF0))
			return 4;
		return 0;
	}

	std::string_view utf8Truncate(std::string_view v, size_t size) {
		auto trunc = v.substr(0, size);
		for (size_t i = 0; i < trunc.length();) {
			auto length = utf8GetCodepointSize(v);
			if (!length)
				return trunc.substr(0, i);

			i += length;
		}

		return trunc;
	}

	class CodepointView {
	private:
		std::string_view _str;
	public:
		class iterator {
		private:
			std::string_view _str;
			size_t _index;

		public:
			//iterator(String str, size_t index) : _str(str), _index(index) {}
			iterator(std::string_view str, size_t index) : _str(str), _index(index) {}

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
					_index = Math::iMin(nextchar - _str.data(), _str.length());
				}
				return *this;
			}
			iterator operator++(int) {
				auto result = *this;
				if (_index < _str.length()) {
					const char* nextchar;
					getNextCodepoint(&_str[_index], &nextchar);
					_index = Math::iMin(nextchar - _str.data(), _str.length());
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
		CodepointView(String str) {
			std::string_view temp(str.c_str());
			_str = utf8Truncate(temp, temp.length());
		}
		iterator begin() const {
			return iterator(_str, 0);
		}
		iterator end() const {
			return iterator(_str, _str.length());
		}
	};
}

#endif
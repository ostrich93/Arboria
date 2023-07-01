#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "Engine.h"

namespace Arboria {

	class Exception {
		public:
			char error[1024];

			const char* getError() {
				return error;
			}

			Exception(const char* text = "") { strcpy(error, text); }
	};
}

#endif


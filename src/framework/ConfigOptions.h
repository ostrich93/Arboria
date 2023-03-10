#ifndef __CONFIG_OPTIONS_H__
#define __CONFIG_OPTIONS_H__

#include "InputBindings.h"

namespace Arboria {
	class ConfigOptions {
		private:
			unsigned int windowWidth;
			unsigned int windowHeight;
			InputBinding* bindings;
		public:
			ConfigOptions() : windowWidth(0), windowHeight(0) {}
			unsigned int getWindowWidth const() { return windowWidth; }
			void setWindowWidth(unsigned int _windowWidth) { windowWidth = _windowWidth; }
			unsigned int getWindowHeight const() { return windowHeight; }
			void setWindowHeight(unsigned int _windowHeight) { windowHeight = _windowHeight; }
	};
}

#endif
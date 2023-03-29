#ifndef __CONFIG_OPTIONS_H__
#define __CONFIG_OPTIONS_H__

namespace Arboria {
	class ConfigOptions {
		private:
			unsigned int screenWidth;
			unsigned int screenHeight;
			unsigned char targetFPS;
		public:
			ConfigOptions() : screenWidth(800), screenHeight(640), targetFPS(60) {}
			unsigned int getScreenWidth const() { return screenWidth; }
			void setScreenWidth(unsigned int _windowWidth) { screenWidth = _windowWidth; }
			unsigned int getScreenHeight const() { return screenHeight; }
			void setScreenHeight(unsigned int _windowHeight) { screenHeight = _windowHeight; }
			unsigned char getTargetFPS() { return targetFPS; }
			void setTargetFPS(unsigned char target) { targetFPS = target; }
	};
}

#endif
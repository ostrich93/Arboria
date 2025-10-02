#ifndef CONFIG_OPTIONS_H
#define CONFIG_OPTIONS_H

namespace Arboria {
	struct ConfigOptions {
		ConfigOptions() = default;
		//make sure there are no copies
		ConfigOptions(const ConfigOptions&) = delete;

		unsigned int defaultDisplay;
		unsigned int screenWidth;
		unsigned int screenHeight;
		unsigned int fullscreenMode;
		unsigned int fullscreenWidth;
		unsigned int fullscreenHeight;
		int drawEngine;
		float windowScale;
		float gamma;
		float brightness;
		float musicVolume;
		float soundVolume;
		unsigned char targetFPS;
		bool useVSync;
		bool showFPS;
		bool minimizeFullscreenOnFocusLoss;
	};

	ConfigOptions& getConfig();
	bool setDefaults();
	bool saveConfig();
}

#endif
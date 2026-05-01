#ifndef SYSTEM_CVARS_H
#define SYSTEM_CVARS_H

#include "CVariableSystem.h"

namespace Arboria {
	struct SystemConfig {
		CVariable* defaultWindowViewportX;
		CVariable* defaultWindowViewportY;
		CVariable* defaultFullscreenWindowViewportX;
		CVariable* defaultFullscreenWindowViewportY;
		CVariable* defaultGamma;
		CVariable* defaultBrightness;
		CVariable* defaultWindowScale;
		CVariable* defaultMusicVolume;
		CVariable* defaultSoundVolume;
		CVariable* defaultLatency;
		CVariable* defaultVSync;

		CVariable* windowViewportX;
		CVariable* windowViewportY;
		CVariable* fullscreenWindowViewportX;
		CVariable* fullscreenWindowViewportY;
		CVariable* gamma;
		CVariable* brightness;
		CVariable* windowScale;
		CVariable* musicVolume;
		CVariable* soundVolume;
		CVariable* latency;
		CVariable* vSync;

		SystemConfig();
		bool initialize();
		bool updateConfigFile();
	};

	extern SystemConfig* systemConfig;
}

#endif
#include "SystemCVars.h"

namespace Arboria {
	
	SystemConfig* systemConfig = NULL;

	SystemConfig::SystemConfig()
	{
		defaultWindowViewportX = new CVariable("defaultWindowViewportX", "640", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		defaultWindowViewportY = new CVariable("defaultWindowViewportY", "480", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultFullscreenWindowViewportX = new CVariable("defaultFullscreenWindowViewportX", "640", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultFullscreenWindowViewportY = new CVariable("defaultFullscreenWindowViewportY", "480", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultWindowColoredBits = new CVariable("defaultWindowColoredBits", "16", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultFullscreenColorBits = new CVariable("defaultFullscreenColoredBits", "16", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		defaultGamma = new CVariable("defaultGamma", "0.500000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		defaultBrightness = new CVariable("defaultBrightness", "1.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultWindowScale = new CVariable("defaultWindowScale", "1.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultMusicVolume = new CVariable("defaultMusicVolume", "1.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultSoundVolume = new CVariable("defaultSoundVolume", "1.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultLatency = new CVariable( "defaultLatency", "1.000000", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );

		windowViewportX = new CVariable("windowViewportX", "0", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		windowViewportY = new CVariable("windowViewportY", "0", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		fullscreenWindowViewportX = new CVariable("fullscreenWindowViewportX", "0", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		fullscreenWindowViewportY = new CVariable("fullscreenWindowViewportY", "0", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		windowColoredBits = new CVariable("windowColoredBits", "0", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		fullscreenColorBits = new CVariable("fullscreenColoredBits", "0", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		gamma = new CVariable("gamma", "0.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		brightness = new CVariable("brightness", "0.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		windowScale = new CVariable("windowScale", "0.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		musicVolume = new CVariable("musicVolume", "0.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		soundVolume = new CVariable("soundVolume", "0.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		latency = new CVariable("latency", "0.000000", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
	}

}
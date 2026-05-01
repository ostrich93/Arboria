#include "SystemCVars.h"
#include "Lexer.h"
#include "../FileSystem.h"

namespace Arboria {
	
	static constexpr const char* configHeader = "[ConfigurationOptions]\\n";

	SystemConfig* systemConfig = NULL;

	SystemConfig::SystemConfig()
	{
		defaultWindowViewportX = new CVariable("defaultWindowViewportX", "640", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		defaultWindowViewportY = new CVariable("defaultWindowViewportY", "360", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultFullscreenWindowViewportX = new CVariable("defaultFullscreenWindowViewportX", "640", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultFullscreenWindowViewportY = new CVariable("defaultFullscreenWindowViewportY", "360", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		
		defaultGamma = new CVariable("defaultGamma", "0.500000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		defaultBrightness = new CVariable("defaultBrightness", "1.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultWindowScale = new CVariable("defaultWindowScale", "1.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultMusicVolume = new CVariable("defaultMusicVolume", "1.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultSoundVolume = new CVariable("defaultSoundVolume", "1.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultLatency = new CVariable( "defaultLatency", "40", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		defaultVSync = new CVariable( "defaultVSync", "1", CVAR_BOOL | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );

		windowViewportX = new CVariable("windowViewportX", "0", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		windowViewportX->setInteger(defaultWindowViewportX->getInteger());
		windowViewportY = new CVariable("windowViewportY", "0", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		windowViewportY->setInteger(defaultWindowViewportY->getInteger());
		fullscreenWindowViewportX = new CVariable("fullscreenWindowViewportX", "0", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		fullscreenWindowViewportX->setInteger(defaultFullscreenWindowViewportX->getInteger());
		fullscreenWindowViewportY = new CVariable("fullscreenWindowViewportY", "0", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "" );
		fullscreenWindowViewportY->setInteger(defaultFullscreenWindowViewportY->getInteger());
		
		gamma = new CVariable("gamma", "0.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		gamma->setFloat(defaultGamma->getFloat());
		brightness = new CVariable("brightness", "0.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		brightness->setFloat(defaultBrightness->getFloat());
		windowScale = new CVariable("windowScale", "1.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		windowScale->setFloat(defaultWindowScale->getFloat());
		musicVolume = new CVariable("musicVolume", "0.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		musicVolume->setFloat(defaultMusicVolume->getFloat());
		soundVolume = new CVariable("soundVolume", "0.000000", CVAR_FLOAT | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		soundVolume->setFloat(defaultSoundVolume->getFloat());
		latency = new CVariable("latency", "0", CVAR_INTEGER | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		latency->setInteger(defaultLatency->getInteger());
		vSync = new CVariable("vSync", "1", CVAR_BOOL | CVAR_CONFIG | CVAR_ENGINE | CVAR_FROMENGINE, "");
		vSync->setBool(defaultVSync->getBool());
	}

	bool SystemConfig::initialize() {
		void* fileBuffer;
		int fLength = readFileFromPhysFS("user.ini", &fileBuffer);

		Lexer src;
		Token t;
		src.loadMemory((char*)fileBuffer, fLength, 0);

		if (!src.expectTokenString("[")) return false;
		if (!src.expectTokenString("ConfigurationOptions")) return false;
		if (!src.expectTokenString("]")) return false;

		while (src.readToken(&t)) {
			if (t == "WindowViewportX") {
				src.expectTokenString("=");
				src.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_INTEGER, &t);
				windowViewportX->setInteger(atoi(t.c_str()));
			}
			if (t == "WindowViewportY") {
				src.expectTokenString("=");
				src.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_INTEGER, &t);
				windowViewportY->setInteger(atoi(t.c_str()));
			}
			if (t == "FullscreenWindowViewportX") {
				src.expectTokenString("=");
				src.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_INTEGER, &t);
				fullscreenWindowViewportX->setInteger(atoi(t.c_str()));
			}
			if (t == "FullscreenWindownViewportY") {
				src.expectTokenString("=");
				src.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_INTEGER, &t);
				fullscreenWindowViewportY->setInteger(atoi(t.c_str()));
			}
			if (t == "Gamma") {
				src.expectTokenString("=");
				src.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_FLOAT, &t);
				gamma->setFloat(atof(t.c_str()));
			}
			if (t == "Brightness") {
				src.expectTokenString("=");
				src.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_FLOAT, &t);
				brightness->setFloat(atof(t.c_str()));
			}
			if (t == "WindowScale") {
				src.expectTokenString("=");
				src.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_INTEGER, &t);
				windowScale->setFloat(atof(t.c_str()));
			}
			if (t == "MusicVolume") {
				src.expectTokenString("=");
				src.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_FLOAT, &t);
				musicVolume->setFloat(atof(t.c_str()));
			}
			if (t == "SoundVolume") {
				src.expectTokenString("=");
				src.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_FLOAT, &t);
				soundVolume->setFloat(atof(t.c_str()));
			}
			if (t == "Latency") {
				src.expectTokenString("=");
				src.expectTokenType(TOKENTYPE_NUMBER, TOKENSUBTYPE_INTEGER, &t);
				latency->setInteger(atoi(t.c_str()));
			}
			if (t == "VSync") {
				src.expectTokenString("=");
				vSync->setBool(src.parseBool());
			}
		}

		return true;
	}

	bool SystemConfig::updateConfigFile()
	{
		//open user.ini file then save parameters
		PHYSFS_File* outFile = PHYSFS_openWrite("user.ini");

		PHYSFS_writeBytes(outFile, (void*)configHeader, strlen(configHeader));
		
		char str[1024];

		sprintf(str, configHeader);
		sprintf(str, "WindowViewportX=%d\\n", windowViewportX->getInteger());
		sprintf(str, "WindowViewportY=%d\\n", windowViewportY->getInteger());
		sprintf(str, "FullscreenWindowViewportX=%d\\n", fullscreenWindowViewportX->getInteger());
		sprintf(str, "FullscreenWindowViewportY=%d\\n", fullscreenWindowViewportY->getInteger());
		sprintf(str, "Gamma=%f\\n", gamma->getFloat());
		sprintf(str, "Brightness=%f\\n", brightness->getFloat());
		sprintf(str, "WindowScale=%d\\n", windowScale->getInteger());
		sprintf(str, "MusicVolume=%f\\n", musicVolume->getFloat());
		sprintf(str, "SoundVolume=%f\\n", soundVolume->getFloat());
		sprintf(str, "Latency=%d\\n", latency->getInteger());
		sprintf(str, "VSync=%d\\n\0", vSync->getInteger());

		int bytesWritten = PHYSFS_writeBytes(outFile, (void*)str, strlen(str));

		PHYSFS_close(outFile);
		return bytesWritten > 0;
	}

}